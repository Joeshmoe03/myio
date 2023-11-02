/*myio.c*/
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

//myopen returns struct
//myread(FILE, *buf, count)

//close call flush
//write call flush?

/* This function attempts to malloc space for character buffer otherewise handles error */
char* trycharmalloc(int size) {
	char* buf = malloc(size);
	if (buf == NULL) {
		perror("malloc");
		exit(1);
	}
	return buf;
}

/* This function attempts to malloc space for MYFILE struct otherwise handles error */
MYFILE* tryFILEmalloc(int size) {
	MYFILE* filep = malloc(sizeof(MYFILE));
	if (filep == NULL) {
		perror("malloc");
		exit(1);
	}
	return filep; 
}

/* myopen returns a new MYFILE struct for later use with other functions */
/* See: https://man7.org/linux/man-pages/man2/open.2.html#ERRORS
 * See: *https://man7.org/linux/man-pages/man3/fopen.3.html */
MYFILE *myopen(const char* path, int flags) {
	int filedesc;
	char* filebuf = trycharmalloc(BUFFER_SIZE);
	MYFILE *filep = tryFILEmalloc(sizeof(MYFILE));
	
	/* If our flags contains O_CREAT or O_TRUNC or both we may assume mode 0666 
	 * else we try to open with O_RDWR xor O_RDONLY xor O_WRONLY without assumed mode. */
	if ((flags & (O_CREAT|O_TRUNC)) == (O_CREAT|O_TRUNC) || O_CREAT || O_TRUNC) {
		filedesc = open(path, flags, 0666);	
	} else {
		filedesc = open(path, flags);
	} //I DON'T THINK WE NEED TO HANDLE PERROR SINCE OPEN AND FOPEN DOESN'T SAY ANYTHING?

	/* We fill our buffer with our filedescriptor and make space */
	//TODO: filep->flags = flags (ADD flags TO typedef struct IN HEADERFILE)
	filep->filedesc = filedesc;
	filep->buf = filebuf;
	filep->bufsize = BUFFER_SIZE;
	filep->bufcount = 0;
	filep->ouroffset = 0;
	filep->useroffset = 0;
	return filep;
}


int myread(MYFILE* filep, char *userbuffer, int count) {

	// //WHAT DO WE DO IF the user is being silly (count is greater than the size they allocated for their userbuffer), give them as much as you can, then die? (this is what read does), 
	// //but we can't give anything with memcpy (instant segfault); memmove()???
		
	/* CASE 1: User asks to read more bytes than the max. our buffer can handle, so we skip buffering 
	  and give them a read with however many bytes they want straight to their buffer */
	if(count > BUFFER_SIZE) {

		/* read() returns negative for error; 0 for EOF (end-of-file) */
		if(read((filep -> filedesc), userbuffer, count) < 0) {
				perror("read");
				return -1;
			}		
		return count;
	}

	/* CASE 2: User asks to read less bytes than the max. our buffer can handle */

	//TODO: Consider writing function for filling found below and in case 2a
	
	/* Initial Read: Completely fill our buffer; based on subsequent myread() calls, memcpy() bytes to userbuffer*/
	if(filep->bufcount == 0) { //hmm... do calc with offset? (or just do ); +offset (P.A to buf and update )
		if(read((filep -> filedesc), filep->buf, filep->bufsize) < 0) {
			perror("read");
			return -1;	
		}		
	}

	/* CASE 2a: Possible RESET if there's bytes remaning in buffer and user attempts to read past the max. our buffer can handle*/ 
	/*POINTER ARITHMETIC HELL*/ //if i'm still operating on same userbuffer, i gotta use arithmetic to move along it
	if(filep->bufcount + count >= BUFFER_SIZE) { //add offset

		/* Copy remaining bytes - what's left in OUR buffer that can help satisfy a portion of this myread() call -
		   from OUR buffer to User's buffer, this will still leave some bytes from THIS myread() call without being read & copied yet */
		memcpy(userbuffer, filep->buf + filep->bufcount, (BUFFER_SIZE - filep->bufcount)); //add offset 

		/* Overwrite OUR buffer and completely fill it with new data */
		if(read((filep -> filedesc), filep->buf, filep->bufsize) < 0) {
			perror("read");
			return -1;	
		}	

		/* Copy the rest of the bytes necessary to fulfill THIS myread() call */
		memcpy(userbuffer + (BUFFER_SIZE - filep->bufcount), filep->buf, count - (BUFFER_SIZE - filep->bufcount));

		/* Reset pointer that indicates our position in buffer */
		filep->bufcount = 0;
		//reset offset as well
		return count;
	}

	/* CASE 2b: User attempts to read an amount of bytes that our buffer can handle; 
	   copy relevant bytes and update pointer that indcates our position in buffer */
	memcpy(userbuffer, filep->buf + filep->bufcount, count); //add offset to position in buffer
	filep->bufcount += count;
	return count;
	
}

// void mywrite(MYFILE* filep, char *inputbuf, int count) {
	
// 	//TODO: HOW TO HANDLE ERRORS? SET ERRNO AND RETURN -1 LIKE write(2) OR DO LIKE fwrite(3)
// 	/* Put all of what our user specified in our MYFILE buffer */
// 	/* If there is enough room in the buffer, do the memcpy into filep->buf */
// 	if (count < filep->bufsize - filep->bufcount) {
// 		memcpy(filep->buf, inputbuf, count);
// 		//TODO: if filep->buf is full do the write
// 	} else {
// 		memcpy(filep->buf, inputbuf, )
// 		//TODO: SOMETHING with memcpy;
// 	}
// 	//Before successful return from write(), the file offset shall be incremented by the number of bytes actually written.
// 	//TODO: OTHERWISE IF FULL, ACTUALLY WRITE

// 	//TODO: Before successful return from write(), the file offset shall be incremented by the number of bytes actually written.
// 	return;
// }

void myflush(MYFILE* filep) { 	//if it's not full, flush it

	//IT WILL never stay full (we read or write at any point that it's full, so no need to check that here(?))
	
	//calls write with however much is left the buffer (into whose buffer tho?)
	if (write(filep->filedesc, filep->buf, filep->bufcount) != 0) {
		perror("write");
	}
}

int myseek(MYFILE *filep, int offset, int whence) {

	//BUFFERING LSEEK ENTAILS HAVING ENOUGH SPACE IN BUFFER TO ARTIFICIALLY CHANGE OUR FILEDESCRIPTOR'S OFFSET WITHOUT HAVING TO INVOKE LSEEK
	//when do we wanna call lseek??
	//similar cases to read	

	/* CASE 1: Try to seek past what's currently in our buffer */
	//I SAY: WHEN WE TRY TO lseek(more than is currently in our buffer OR more than buffersize? (same for read & write?)) -> should just give in

	/*Just need one???*/ /*W!!!!!*/ //WORKS FOR NOW 
	if(offset > BUFFER_SIZE || filep->bufcount + offset > BUFFER_SIZE) { //whether we do seek or cur we're cooked
		if((filep->useroffset = lseek(filep->filedesc, offset, whence)) == -1) {
			perror("lseek");
			}

		//DON'T THINK I NEED THIS; DON'T care about remainder or capturing what we lseek before new buffer is initialized
		// printf("in here h\n");
		// printf("%d\n", filep->useroffset);
		// printf("%d\n", (filep->useroffset/BUFFER_SIZE));
		// filep->ouroffset = filep->useroffset - (filep->useroffset/BUFFER_SIZE) * BUFFER_SIZE ; //do some smart calculation; //should be capped out at BUFFER_size (will give us position in our buffer where our data will be)
		// printf("%d\n", filep->ouroffset);

		return filep->useroffset;
	}


	/* CASE 2: Try to seek within the limit of our buffer*/

	//could go out with seek_cur if (filep->bufcount + offset > BUFFER_SIZE)
	// if(filep->bufcount + offset > BUFFER_SIZE) {
		
	// }


	//do need to update our bufcount
	

	
	// /* Case 1a: seek_set */
	// if(whence == SEEK_SET) {
	// 	if(offset > BUFFER_SIZE) {
	// 		if(lseek(filep->filedesc, offset, SEEK_SET) == -1) {
	// 		perror("lseek");
	// 		}
	// 	}
	// }

	// /* Case 1b: see_cur */ //how does it know it's current location //how do I keep track of that //has internal offset thing going on
	// //i dont need to be aware of it
	// if(whence == SEEK_CUR) {
	// 	if(offset + filep> filep->bufcount) {
	// 		if(lseek(filep->filedesc, offset, SEEK_SET) == -1) {
	// 		perror("lseek");
	// 		}
	// 	}
	// }
		
		// filep->bufcount = offset; //eh capped at buffsize
		// filep->useroffset = offset;

		return -1;

		
	}




/* CASE 2: SEEK_CUR */

	// if(whence == SEEK_CUR) {
	// 	filep->bufcount += offset; //eh capped at buffsize
	// 	filep->useroffset += offset;
	// }

	// return 1;

	//"fake" useroffset: where they "are" in a file

	//for return purposes:

	//fake + offset

	//fake = offset	

	//our offset: where we are in our buffer (filep->bufcount) //MUST update this

//}

int myclose(MYFILE* filep) {
	//call flush
	//myflush()

	//free everything
	free(filep->buf);
	free(filep); //free struct
	

	//call close()

	return 0;
}



// int main(int argc, char *argv[]) {
// 	//MYFILE* filep = myopen("hello.txt", O_CREAT | O_TRUNC | O_RDWR);
// 	//mywrite(filep, "Hello", 2);
// }


