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

#define BUFFER_SIZE 25

/* This function attempts to malloc space for character IObuffer otherewise handles error */
char* trycharmalloc(int size) {
	char* IObuf = malloc(size);
	if (IObuf == NULL) {
		free(IObuf);
		perror("malloc"); //TODO: CORRECT HANDLING?
		exit(1);
	}
	return IObuf;
}

/* This function attempts to malloc space for MYFILE struct otherwise handles error */
MYFILE* tryFILEmalloc(int size) {
	MYFILE* filep = malloc(sizeof(MYFILE));
	if (filep == NULL) {
		free(filep);
		perror("malloc"); //TODO CORRECT HANDLING?
		exit(1);
	}
	return filep; 
}

/* myopen returns a new MYFILE struct for later use with other functions */
/* See: https://man7.org/linux/man-pages/man2/open.2.html#ERRORS
 * See: *https://man7.org/linux/man-pages/man3/fopen.3.html */
MYFILE *myopen(const char* path, int flags) {
	int filedesc;
	char* fileIObuf = trycharmalloc(BUFFER_SIZE);
	MYFILE *filep = tryFILEmalloc(sizeof(MYFILE));
	
	/* If our flags contains O_CREAT or O_TRUNC or both we may assume mode 0666 
	 * else we try to open with O_RDWR xor O_RDONLY xor O_WRONLY without assumed mode. */
	if ((flags & (O_CREAT|O_TRUNC)) == (O_CREAT|O_TRUNC) || 
		(flags & (O_CREAT|O_TRUNC)) == O_CREAT || 
		(flags & (O_CREAT|O_TRUNC)) == O_TRUNC) {
		filedesc = open(path, flags, 0666);
		if (filedesc == -1) {
			return NULL;
		}
	} else {
		filedesc = open(path, flags);
		if (filedesc == -1) {
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


int myread(MYFILE* filep, char *userIObuffer, int count) {

	/* bytes left in our buffer that have not been memcpy'd yet*/
	int IObufspace = BUFFER_SIZE - filep->IOoffset;

	/* bytes we STILL need to give user after GIVING them what was left in our buffer */
	int remainder = 0; //still need to give;
		
	/* CASE 1: User asks to read more bytes (or same amount) than the max. our IObuffer can handle AND our buffer is EMPTY, so we skip IObuffering 
	  and give them a read with however many bytes they want straight to their IObuffer */
	if(count >= BUFFER_SIZE && filep->IOoffset == 0) {

		/* read() returns negative for error; 0 for EOF (end-of-file) */
		if(read((filep -> filedesc), userIObuffer, count) < 0) {
				perror("read");
				return -1;
		}
		filep->IOoffset = 0; //making it explict for readibility's sake
		filep->wasread = 1;		
		return count; 
	}

	/* CASE 2: User asks to read less bytes than the max. our IObuffer can handle (our buffer may be contain something or be empty) */

	//TODO: Consider writing function for filling found below and in case 2a
	
	/* Initial Read: Completely fill our IObuffer*/ /*don't wanna do this when if count is == 0 */
	if(filep->IOoffset == 0 && count != 0) { //|| filep->IOoffset == BUFFER_SIZE//this or do an if by the bottom (line __)
		if(read((filep -> filedesc), filep->IObuf, filep->IOsiz) < 0) {
			perror("read");
			return -1;	
		}		
	}

	/* CASE 2a: Possible RESET if there's bytes remaning in IObuffer and user attempts to read past the max. our IObuffer can handle*/ 
	/*POINTER ARITHMETIC HELL*/ //if i'm still operating on same userIObuffer, i gotta use arithmetic to move along it
	if(filep->IOoffset + count > BUFFER_SIZE) { /*if there's smth in buffer and we are going to go out of bounds*/
		/* Copy remaining bytes - what's left in OUR IObuffer that can help satisfy a portion of this myread() call -
		   from OUR IObuffer to User's IObuffer, this will prevent us from leaving some bytes from THIS myread() call without being read & copied yet */
		memcpy(userIObuffer, filep->IObuf + filep->IOoffset, (BUFFER_SIZE - filep->IOoffset)); //add offset 

		/* Calculate remainder */
		remainder = count - (BUFFER_SIZE - filep->IOoffset);

		// + does this need to be applied to mywrite 
		// + do we need to keep remainder in the struct??*/
		if (remainder > BUFFER_SIZE) { /*just go ahead and read the remainder (basically case 1)*/
			/* read() returns negative for error; 0 for EOF (end-of-file) */
			if(read((filep -> filedesc), userIObuffer + (BUFFER_SIZE - filep->IOoffset), remainder) < 0) {
					perror("read");
					return -1;
				}		
			filep->wasread = 1;
			filep->IOoffset = 0;
			return count;
		}

		/* Overwrite OUR IObuffer and completely fill it with new data */
		if(read((filep -> filedesc), filep->IObuf, filep->IOsiz) < 0) {
			perror("read");
			return -1;	
		}	
		/* Copy the rest of the bytes necessary to fulfill THIS myread() call */
		memcpy(userIObuffer + (BUFFER_SIZE - filep->IOoffset), filep->IObuf, remainder);

		/* Reset pointer that indicates our position in IObuffer to the REMAINDER (our position in our NEW resetted buffer)*/
		filep->IOoffset = remainder;
		filep->wasread = 1;
		return count;
	}

	/* CASE 2b: User attempts to read an amount of bytes that our IObuffer can handle; 
	   copy relevant bytes and update pointer that indcates our position in IObuffer */
	memcpy(userIObuffer, filep->IObuf + filep->IOoffset, count); 
	filep->IOoffset += count; /*update IOoffset*/
	/* HANDLES CASE WHERE WE JUST FILLED OUR BUFFER (ex: buffer size = 25, read(24) then read(1))*/
	if(filep->IOoffset == BUFFER_SIZE) {
		filep->IOoffset = 0;
		return count;
	}	
	filep->wasread = 1;
	return count;
	
}

int mywrite(MYFILE* filep, const char *inbuf, int count) {
	int nbytetomemcpy = 0;
	int inbufoffset = 0;

	// if(wasread == 1) {
		
	// };

	/* Update fileoffset by the fake amount that the user wanted to write */
	int IObufspace = filep->IOsiz - filep->IOoffset;

	filep->fileoffset += count;
	nbytetomemcpy = count;

	/* We check that our flags are fine */
	if ((filep->flags & (O_RDWR|O_WRONLY)) != O_RDWR && (filep->flags & (O_RDWR|O_WRONLY)) != O_WRONLY) { //TODO: CHECK CORRECTNESS
		exit(1);
	}

	/* If we want to write something that exceeds the capacity of our buffer, might as well call it directly */
	if (count > filep->IOsiz) {
		
		/* Flush any previously buffered items and then call write as we write something larger than even our buffer size */
		myflush(filep);
		filep->IOoffset = 0;
		if (write(filep->filedesc, inbuf, count) < 0) {
			filep->IOoffset = 0;
			return EOF;
		}
		nbytetomemcpy = 0;
	}
	
	/* If we attempt to write larger than buffer space including that we have stuff in the buffer from before */
	else if (filep->IOoffset + count >= filep->IOsiz) {

		/* Fill all possible buffer space, flush, and calculate how much more we have to put in buff to enter default case */
		nbytetomemcpy = IObufspace;
		memcpy(filep->IObuf + filep->IOoffset, inbuf, nbytetomemcpy);
		filep->IOoffset = filep->IOsiz;
		myflush(filep);
		nbytetomemcpy = count - nbytetomemcpy;
		inbufoffset += nbytetomemcpy;
		filep->IOoffset = 0;
	}

	/* Default case of put everything specified in buf since it is not more than bufsize and return fake count */
	memcpy(filep->IObuf + filep->IOoffset, inbuf + inbufoffset, nbytetomemcpy);
	filep->IOoffset += nbytetomemcpy;
	filep->waswrite = 1;
	return count;
}

int myflush(MYFILE* filep) {
	
	/* We check that our flags are fine */
	if ((filep->flags & (O_RDWR|O_WRONLY)) != O_RDWR && (filep->flags & (O_RDWR|O_WRONLY)) != O_WRONLY) { //TODO: CHECK CORRECTNESS
		filep->IOoffset = 0;
		return EOF;
	}
	
	/* calls write with however much is left the IObuffer */
	if (write(filep->filedesc, filep->IObuf, filep->IOoffset) < 0) {
		filep->IOoffset = 0;
		return EOF;
	}

	/* Returns 0 on success otherwise previously EOF is returned */
	filep->IOoffset = 0;
	return 0;
}

int myseek(MYFILE *filep, int offset, int whence) {
	
	/* CASE 1: SEEK_SET */
	if(whence == SEEK_SET) {
		if((filep->fileoffset = lseek(filep->filedesc, offset, SEEK_SET)) < 0) {
			perror("lseek");
		}
		filep->fileoffset = offset;
	}

	/* Case 2: SEEK_CUR */
	else if(whence == SEEK_CUR) {
		if((filep->fileoffset = lseek(filep->filedesc, filep->fileoffset + offset, SEEK_SET)) < 0) {
			perror("lseek");
		}
	}

	else {
		//error message for bad flag
	}

	return filep->fileoffset;
}

int myclose(MYFILE* filep) {
	//call flush
	//myflush()

	if(close(filep->filedesc < 0)) {
		perror("close");
	}

	//free everything
	free(filep->IObuf);
	free(filep); //free struct
	
	return 0;
}