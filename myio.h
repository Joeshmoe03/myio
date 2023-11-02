/*
 * myio.h
 */

#ifndef __MYIO_H
#define __MYIO_H

typedef struct {
	int filedesc;
	char *buf;
	int bufsize;
	int bufcount;
	int useroffset;
	int ouroffset;
	//SOMETHING;
} MYFILE;

//_______________________________________________________________________________________

/* Function prototype for myopen */
MYFILE* myopen(const char* path, int flags);

void mywrite(MYFILE* filep, char *buf, int count);

int myclose();

int myread(MYFILE* filep, char *userbuffer, int count);

int myseek(MYFILE *filep, int offset, int whence);

void myflush();



#endif /* __MYIO_H*/