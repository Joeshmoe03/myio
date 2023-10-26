/*myio.c*/
#include <stdio.h>
#include <libc.h>
#include <myio.h>
#include <fcntl.h>
#include <sys/stat.h>

/* How many bytes we should actually work with REALLY to not use syscalls redundantly */
#define BUFFER_SIZE 4096; //TODO: INCLUDE IN HEADER?

typedef struct {
	int filedesc;
	int buffsize;
	int offset;
	int //SOMETHING;
} MYFILE

//_______________________________________________________________________________________

/* Function prototype for myopen(2) */
MYFILE *myopen(const char* path, int flags, mode_t mode);

int myclose();

int myread();

int mywrite();

int myseek();

int myflush();

//_______________________________________________________________________________________

//myopen returns struct
//myread(FILE, *buf, count)

//close call flush
//write call flush?


//write takes in df and #buf, count

//const int O_CREAT = 0;
//const int O_WRONLY = 0;
//const int O_RDONLY = 0;
//const int O_RDWR = 0;
//const int O_TRUNC = 0;

MYFILE *myopen(const char path, int flags, mode_t mode) {
	int filedesc;
    MYFILE 

	return //RETURN MYFILE STRUCT
}

MYFILE* 

