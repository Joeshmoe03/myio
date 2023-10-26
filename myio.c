/*myio.c*/
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "myio.h"
#include <stdlib.h>

#define BUFFER_SIZE 4096

//myopen returns struct
//myread(FILE, *buf, count)

//close call flush
//write call flush?

MYFILE *myopen(const char* path, int flags) { 
	int filedesc;
    
	/* MYFILE is a struct that we made in myio.h: contains relevant info to our file and corresponding buffer */
	MYFILE *filep = malloc(sizeof(MYFILE));
	
	//TODO: HANDLE OUR FLAGS
	switch(flags) {
		case O_CREAT:
			filedesc = open(path, O_CREAT, 0666); //TODO
			break;
		case O_WRONLY:
			filedesc = open(path, O_WRONLY); //TODO
			break;
		case O_RDONLY:
			filedesc = open(path, O_RDONLY); //TODO
			break;
		case O_RDWR:
			filedesc = open(path, O_RDWR); //TODO
			break;
		case O_TRUNC:
			filedesc = open(path, O_TRUNC, 0666); //TODO
			break;
		default:
			printf("in default");
			break;
		
		void* filebuff = malloc(BUFFER_SIZE); //TODO: IS THIS CORRECT TYPE? SHOULD WE EVEN DO HERE?

		filep->filedesc = filedesc;
		filep->buff = filebuff;
		filep->buffsize = BUFFER_SIZE;
		filep->offset = 0;
		filep->useroffset = 0;
		//TODO: ANY OTHER STRUCT ELEMENTS?
	}
	return filep;
}
