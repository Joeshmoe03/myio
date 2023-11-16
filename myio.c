/* myio.c */
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "myio.h"

#define BUFFER_SIZE 4096

/* This function attempts to malloc space for character IObuffer otherewise handles error */
void* trymalloc(int size) {
	char* bufp = malloc(size);
	if(bufp == NULL) {
		free(bufp);
		return NULL;
	}
	return bufp;
}

/* myopen returns a new MYFILE struct for later use with other functions */
/* See: https://man7.org/linux/man-pages/man2/open.2.html#ERRORS
 * See: *https://man7.org/linux/man-pages/man3/fopen.3.html */
MYFILE *myopen(const char* path, int flags) {
	int filedesc;
	char* fileIObuf = (char *)trymalloc(BUFFER_SIZE);
	MYFILE *filep = (MYFILE *)trymalloc(sizeof(MYFILE));
	
	/* If our flags contains O_CREAT or O_TRUNC or both we may assume mode 0666 
	 * else we try to open with O_RDWR xor O_RDONLY xor O_WRONLY without assumed mode. */
	if((flags & (O_CREAT|O_TRUNC)) == (O_CREAT|O_TRUNC) || 
		(flags & (O_CREAT|O_TRUNC)) == O_CREAT || 
		(flags & (O_CREAT|O_TRUNC)) == O_TRUNC) {
		filedesc = open(path, flags, 0666);
		if(filedesc == -1) {
			return NULL;
		}
	} else {
		filedesc = open(path, flags);
		if(filedesc == -1) {
			return NULL;
		}
	}

	/* We fill our IObuffer with our filedescriptor and make space */
	filep->flags = flags;
	filep->filedesc = filedesc;
	filep->IObuf = fileIObuf;
	filep->IOsiz = BUFFER_SIZE;
	filep->IOoffset = 0;
	filep->fileoffset = 0;
	filep->wasread = 0;
	filep->waswrite = 0;
	return filep;
}

int myread(MYFILE* filep, char* outbuf, int count) {
	int outbufoffset = 0;
	int nbytetoread = count;
	int nbytewasread = 0;
	
	filep->fileoffset += count;
	
	/* Reset some flag when moving between reads and writes. */
	filep->waswrite = 1;
	if(filep->waswrite == 1) {
		myflush(filep);
		//filep->waswrite = 0;
		filep->wasread = 1; //TODO: TESTING
	}	

	/* We check that our flags are fine */
	if((filep->flags & (O_RDWR|O_RDONLY)) != O_RDWR && (filep->flags & (O_RDWR|O_RDONLY)) != O_RDONLY) {
		return -1;
	}

	/* If user wants to read an obscene amount, let them: first extract rest from IObuf and then use syscall directly */
	if(count > filep->IOsiz) {
		nbytetoread = filep->IOsiz - filep->IOoffset;
		if(filep->IOoffset == 0 /*&& filep->wasread == 1*/) {
			nbytetoread = 0;
		}
		memcpy(outbuf + outbufoffset, filep->IObuf + filep->IOoffset, nbytetoread);
		filep->IOoffset = 0;
		outbufoffset += nbytetoread;
		nbytetoread = count - nbytetoread;

		/* Attempt a read with error handling directly on outbuf with no buffering (IObuf) */
		if((nbytewasread = (int)read(filep->filedesc, outbuf + outbufoffset, nbytetoread)) < 0) {
			return -1;
		}
		
		/* Update nbytetoread: nothing left to read for later as we used syscall directly */
		outbufoffset += nbytetoread;
		nbytetoread = 0;
		/*filep->wasread = 1;*/
	}

	/* If count is not larger than IObuf, but factoring items already in IObuf, it is */
	else if(count + filep->IOoffset > filep->IOsiz) {

		/* Lets first read the rest of buffered stuff in IObuf to the user's buffer and update some
		* buffer tracking variables: offset is now buffer size since we exhausted our buffer, nbytetoread
		* is the count factoring what we just read from the buffer */
		nbytetoread = filep->IOsiz - filep->IOoffset;
		memcpy(outbuf + outbufoffset, filep->IObuf + filep->IOoffset, nbytetoread);
		filep->IOoffset = 0;
		outbufoffset += nbytetoread;
		nbytetoread = count - nbytetoread;
		/*filep->wasread = 1;*/
	}
	//printf("%d\n", filep->wasread);
	/* If our IObuf is either empty on start or has exhausted everything to be read into outbuf we read again
	* This also takes care of scenario of rebuffering when count + IOoffset was larger than buffer size and we still
	* need to read a little bit beyond the bounds of IObuf to the user's buffer	*/
	if(filep->IOoffset == 0 && nbytetoread != 0/*&& filep->wasread == 1*/) {
		if((nbytewasread = (int)read(filep->filedesc, filep->IObuf + filep->IOoffset, filep->IOsiz)) < 0) {
			return -1;
		}
		//filep->wasread = 0;
		//printf("%d\n", filep->wasread);
	}

	/* If read 0, its fine as nbytetoread = 0 then. Will do any remaining reads left to do after the count + IOoffset > IOsiz case.
	* Also handles the scenario where user's call to read is not substantial enough to warrant a new syscall and just gets it from buffered
	* previous read syscall */
	memcpy(outbuf + outbufoffset, filep->IObuf + filep->IOoffset, nbytetoread);
	filep->IOoffset += nbytetoread;

	if(nbytewasread > count) {
		nbytewasread = count;
	}
	return nbytewasread;
}

int mywrite(MYFILE* filep, const char *inbuf, int count) {
	int nbytetomemcpy = count;
	int inbufoffset = 0;

	/* This handles logic for when I decide to move from read to write */
	filep->waswrite = 1;
	if(filep->wasread == 1) {
		//filep->waswrite = 1;
		filep->IOoffset = 0; //TEST
		filep->wasread = 0;
	}

	/* Update fileoffset by the fake amount that the user wanted to write */
	int IObufspace = filep->IOsiz - filep->IOoffset;
	filep->fileoffset += count;

	/* We check that our flags are fine */
	if((filep->flags & (O_RDWR|O_WRONLY)) != O_RDWR && (filep->flags & (O_RDWR|O_WRONLY)) != O_WRONLY) {
		return -1;
	}

	/* If we want to write something that exceeds the capacity of our buffer, might as well call it directly */
	if(count > filep->IOsiz) {
		
		/* Flush any previously buffered items and then call write as we write something larger than even our buffer size */
		myflush(filep);
		filep->IOoffset = 0;
		if(write(filep->filedesc, inbuf, count) < 0) {
			return -1;
		}
		nbytetomemcpy = 0;
	}
	
	/* If we attempt to write larger than buffer space factoring in that we have stuff in the buffer from before */
	else if(filep->IOoffset + count > filep->IOsiz) {

		/* Fill all possible buffer space, flush, and calculate how much more we have to put in write buffer for entering default case */
		nbytetomemcpy = IObufspace;
		memcpy(filep->IObuf + filep->IOoffset, inbuf, nbytetomemcpy);
		filep->IOoffset = filep->IOsiz;
		myflush(filep);
		nbytetomemcpy = count - nbytetomemcpy;
		inbufoffset += IObufspace;
		filep->IOoffset = 0;
	}

	/* Default case of put everything specified in buf since it is not more than bufsize and return fake count */
	memcpy(filep->IObuf + filep->IOoffset, inbuf + inbufoffset, nbytetomemcpy);
	filep->IOoffset += nbytetomemcpy;
	return count;
}

int myflush(MYFILE* filep) {
	
	filep->waswrite = 0;
	filep->wasread = 0;

	/* We check that our flags are fine */
	if((filep->flags & (O_RDWR|O_WRONLY)) != O_RDWR && (filep->flags & (O_RDWR|O_WRONLY)) != O_WRONLY) {
		filep->IOoffset = 0;
		return -1;
	}
	
	/* calls write with however much is left the IObuffer */
	if(write(filep->filedesc, filep->IObuf, filep->IOoffset) < 0) {
		filep->IOoffset = 0;
		return -1;
	}

	/* Returns 0 on success */
	filep->IOoffset = 0;
	return 0;
}

int myseek(MYFILE *filep, int offset, int whence) {	

	if(whence != SEEK_SET && whence != SEEK_CUR) {
		return -1;
	}

	/* We flush if previous was a write */
	if(filep->waswrite == 1) {
		filep->waswrite = 0;
		myflush(filep);
	}
	
	/* CASE 1: SEEK_SET */
	if(whence == SEEK_SET) {
		if((filep->fileoffset = lseek(filep->filedesc, offset, SEEK_SET)) < 0) {
			return -1;
		}
		filep->fileoffset = offset;
		filep->IOoffset = 0;
	}

	/* Case 2: SEEK_CUR */
	else if(whence == SEEK_CUR) {
		if((filep->fileoffset = lseek(filep->filedesc, filep->fileoffset + offset, SEEK_SET)) < 0) {
			return -1;
		}
		filep->IOoffset = 0;
	}
	return filep->fileoffset;
}

int myclose(MYFILE* filep) {

	/* If the last thing specified was a write, just flush */
	if(filep->waswrite == 1) {
		myflush(filep);
	}	

	/* Actually do the close */
	if(close(filep->filedesc) < 0) {
		return -1;
	}

	/* Free everything */
	free(filep->IObuf);
	free(filep);
	
	return 0;
}
