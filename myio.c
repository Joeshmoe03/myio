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

#define IObufFER_SIZE 100

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
	char* fileIObuf = trycharmalloc(IObufFER_SIZE);
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
	filep->IOsiz = IObufFER_SIZE;
	filep->IOoffset = 0;
	filep->fileoffset = 0;
	return filep;
}


int myread(MYFILE* filep, char *userIObuffer, int count) {

	// //WHAT DO WE DO IF the user is being silly (count is greater than the size they allocated for their userIObuffer), give them as much as you can, then die? (this is what read does), 
	// //but we can't give anything with memcpy (instant segfault); memmove()???
		
	/* CASE 1: User asks to read more bytes than the max. our IObuffer can handle, so we skip IObuffering 
	  and give them a read with however many bytes they want straight to their IObuffer */
	if(count > IObufFER_SIZE) {

		/* read() returns negative for error; 0 for EOF (end-of-file) */
		if(read((filep -> filedesc), userIObuffer, count) < 0) {
				perror("read");
				return -1;
			}		
		return count;
	}

	/* CASE 2: User asks to read less bytes than the max. our IObuffer can handle */

	//TODO: Consider writing function for filling found below and in case 2a
	
	/* Initial Read: Completely fill our IObuffer; based on subsequent myread() calls, memcpy() bytes to userIObuffer*/
	if(filep->IOoffset == 0) { //hmm... do calc with offset? (or just do ); +offset (P.A to IObuf and update )
		if(read((filep -> filedesc), filep->IObuf, filep->IOsiz) < 0) {
			perror("read");
			return -1;	
		}		
	}

	/* CASE 2a: Possible RESET if there's bytes remaning in IObuffer and user attempts to read past the max. our IObuffer can handle*/ 
	/*POINTER ARITHMETIC HELL*/ //if i'm still operating on same userIObuffer, i gotta use arithmetic to move along it
	if(filep->IOoffset + count >= IObufFER_SIZE) { //add offset

		/* Copy remaining bytes - what's left in OUR IObuffer that can help satisfy a portion of this myread() call -
		   from OUR IObuffer to User's IObuffer, this will still leave some bytes from THIS myread() call without being read & copied yet */
		memcpy(userIObuffer, filep->IObuf + filep->IOoffset, (IObufFER_SIZE - filep->IOoffset)); //add offset 

		/* Overwrite OUR IObuffer and completely fill it with new data */
		if(read((filep -> filedesc), filep->IObuf, filep->IOsiz) < 0) {
			perror("read");
			return -1;	
		}	

		/* Copy the rest of the bytes necessary to fulfill THIS myread() call */
		memcpy(userIObuffer + (IObufFER_SIZE - filep->IOoffset), filep->IObuf, count - (IObufFER_SIZE - filep->IOoffset));

		/* Reset pointer that indicates our position in IObuffer */
		filep->IOoffset = 0;
		//reset offset as well
		return count;
	}

	/* CASE 2b: User attempts to read an amount of bytes that our IObuffer can handle; 
	   copy relevant bytes and update pointer that indcates our position in IObuffer */
	memcpy(userIObuffer, filep->IObuf + filep->IOoffset, count); //add offset to position in IObuffer
	filep->IOoffset += count;
	return count;
	
}

int mywrite(MYFILE* filep, const char *inbuf, int count) {
	int nbytetomemcpy = 0;
	int inbufoffset = 0;

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

	//IObufFERING LSEEK ENTAILS HAVING ENOUGH SPACE IN IObufFER TO ARTIFICIALLY CHANGE OUR FILEDESCRIPTOR'S OFFSET WITHOUT HAVING TO INVOKE LSEEK
	//when do we wanna call lseek??
	//similar cases to read	

	/* CASE 1: Try to seek past what's currently in our IObuffer */
	//I SAY: WHEN WE TRY TO lseek(more than is currently in our IObuffer OR more than IObuffersize? (same for read & write?)) -> should just give in

	/*Just need one???*/ /*W!!!!!*/ //WORKS FOR NOW 
	if(offset > IObufFER_SIZE || filep->IOoffset + offset > IObufFER_SIZE) { //whether we do seek or cur we're cooked
		if((filep->fileoffset = lseek(filep->filedesc, offset, whence)) == -1) {
			perror("lseek");
		}

		//DON'T THINK I NEED THIS; DON'T care about remainder or capturing what we lseek before new IObuffer is initialized
		// printf("in here h\n");
		// printf("%d\n", filep->fileoffset);
		// printf("%d\n", (filep->fileoffset/IObufFER_SIZE));
		// filep->ouroffset = filep->fileoffset - (filep->fileoffset/IObufFER_SIZE) * IObufFER_SIZE ; //do some smart calculation; //should be capped out at IObufFER_size (will give us position in our IObuffer where our data will be)
		// printf("%d\n", filep->ouroffset);

		return filep->fileoffset;
	}


	/* CASE 2: Try to seek within the limit of our IObuffer*/

	//could go out with seek_cur if (filep->IOoffset + offset > IObufFER_SIZE)
	// if(filep->IOoffset + offset > IObufFER_SIZE) {
		
	// }


	//do need to update our IOoffset
	

	
	/* Case 1a: seek_set */
	if(whence == SEEK_SET) {
		if(offset > IObufFER_SIZE) {
			if(lseek(filep->filedesc, offset, SEEK_SET) == -1) {
			perror("lseek");
			}
		}
	}

	// /* Case 1b: see_cur */ //how does it know it's current location //how do I keep track of that //has internal offset thing going on
	// //i dont need to be aware of it
	// if(whence == SEEK_CUR) {
	// 	if(offset + filep > filep->IOoffset) {
	// 		if(lseek(filep->filedesc, offset, SEEK_SET) == -1) {
	// 		perror("lseek");
	// 		}
	// 	}
	// }
		
		// filep->IOoffset = offset; //eh capped at IObuffsize
		// filep->fileoffset = offset;

		return -1;

		
	}




/* CASE 2: SEEK_CUR */

	// if(whence == SEEK_CUR) {
	// 	filep->IOoffset += offset; //eh capped at IObuffsize
	// 	filep->fileoffset += offset;
	// }

	// return 1;

	//"fake" fileoffset: where they "are" in a file

	//for return purposes:

	//fake + offset

	//fake = offset	

	//our offset: where we are in our IObuffer (filep->IOoffset) //MUST update this

//}

int myclose(MYFILE* filep) {
	//call flush???
	myflush(filep);

	//free everything
	free(filep->IObuf);
	free(filep); //free struct
	

	//call close()

	return 0;
}



// int main(int argc, char *argv[]) {
// 	//MYFILE* filep = myopen("hello.txt", O_CREAT | O_TRUNC | O_RDWR);
// 	//mywrite(filep, "Hello", 2);
// }


