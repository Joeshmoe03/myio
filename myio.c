/*myio.c*/
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "myio.h"
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

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
	filep->offset = 0;
	filep->useroffset = 0;
	return filep;
}

void mywrite(MYFILE* filep, char *inputbuf, int count) {
	
	//TODO: HOW TO HANDLE ERRORS? SET ERRNO AND RETURN -1 LIKE write(2) OR DO LIKE fwrite(3)
	/* Put all of what our user specified in our MYFILE buffer */
	/* If there is enough room in the buffer, do the memcpy into filep->buf */: 
	if (count < filep->bufsize - filep->bufcount) {
		memcpy(filep->buf, inputbuf, count);
		//TODO: if filep->buf is full do the write
	} else {
		memcpy(filep->buf, inputbuf, )
		//TODO: SOMETHING with memcpy;
	}
	//Before successful return from write(), the file offset shall be incremented by the number of bytes actually written.
	//TODO: OTHERWISE IF FULL, ACTUALLY WRITE

	//TODO: Before successful return from write(), the file offset shall be incremented by the number of bytes actually written.
	return;
}

int main(int argc, char *argv[]) {
	MYFILE* filep = myopen("hello.txt", O_CREAT | O_TRUNC | O_RDWR);
	mywrite(filep, "Hello", 2);
}

