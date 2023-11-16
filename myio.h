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
	int fileoffset;
	int IOoffset;
	int waswrite;
	int wasread;
} MYFILE;

/* Function prototype for myopen */
MYFILE* myopen(const char* path, int flags);

int mywrite(MYFILE* filep, const char *inbuf, int count);

int myclose(MYFILE *filep);

int myread(MYFILE* filep, char *userbuffer, int count);

int myflush(MYFILE *filep);

int myseek(MYFILE *filep, int offset, int whence);

#endif /* __MYIO_H*/
