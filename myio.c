/*myio.c*/
#include <stdio.h>
#include <libc.h>
#include <myio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "myio.h"

/* How many bytes we should actually work with REALLY to not use syscalls redundantly */
#define BUFFER_SIZE 4096

//myopen returns struct
//myread(FILE, *buf, count)

//close call flush
//write call flush?


MYFILE *myopen(const char path, int flags, mode_t mode) {
	int filedesc;
    MYFILE 

	return //RETURN MYFILE STRUCT (or a pointer to it idk) //YEA A POINTER TO IT
}

MYFILE 




