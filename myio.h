/*
 * myio.h
 */

#ifndef __MYIO_H
#define __MYIO_H

typedef struct {
	int flags;
	int filedesc;
	char *IObuf;
	int IOsiz;
	int IOcnt;
	int fileoffset;
	int IOoffset;
	//SOMETHING;
} MYFILE;

//_______________________________________________________________________________________

/* Function prototype for myopen */
MYFILE* myopen(const char* path, int flags);

int mywrite(MYFILE* filep, const char *inbuf, int count);

int myclose();

int myread(MYFILE* filep, char *userbuffer, int count);

int myseek(MYFILE *filep, int offset, int whence);

void myflush();



#endif /* __MYIO_H*/
