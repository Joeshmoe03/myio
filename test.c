
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "myio.h"

#define BUFFER_SIZE 100

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
	if ((flags & (O_CREAT|O_TRUNC)) == (O_CREAT|O_TRUNC) || 
		(flags & (O_CREAT|O_TRUNC)) == O_CREAT || 
		(flags & (O_CREAT|O_TRUNC)) == O_TRUNC) {
		filedesc = open(path, flags, 0666);
	} else {
		filedesc = open(path, flags);
	} //I DON'T THINK WE NEED TO HANDLE PERROR SINCE OPEN AND FOPEN DOESN'T SAY ANYTHING?

	/* We fill our buffer with our filedescriptor and make space */
	filep->flags = flags;
	filep->filedesc = filedesc; //?
	filep->IObuf = filebuf;
	filep->IOsiz = BUFFER_SIZE;
	filep->IOcnt = 0;
	filep->IOoffset = 0;
	filep->fileoffset = 0;
	return filep;
}

int mywrite(MYFILE* filep, const char *inbuf, int count) {
	int nbytetomemcpy = 0;
	int inbufoffset = 0;

	/* Update fileoffset by the fake amount that the user wanted to write */
	int IObufspace = filep->IOsiz - filep->IOcnt;

	filep->fileoffset += count;
	nbytetomemcpy = count;

	/* We check that our flags are fine */
	if ((filep->flags & (O_RDWR|O_WRONLY)) != O_RDWR && (filep->flags & (O_RDWR|O_WRONLY)) != O_WRONLY) { //TODO: CHECK CORRECTNESS
		exit(1);
	}

	/* If we want to write something that exceeds the capacity of our buffer, might as well call it directly */
	if (count > filep->IOsiz) {
		
		/* Flush any previously buffered items and then call write as we write something larger than even our buffer size */
		write(filep->filedesc, filep->IObuf, filep->IOcnt); //myflush(filep);
		if (write(filep->filedesc, inbuf, count+2) < 0) {
			return -1;
		}
		nbytetomemcpy = 0;
		filep->IOoffset = 0;
		filep->IOcnt = 0;
	}
	
	/* If we attempt to write larger than buffer space including that we have stuff in the buffer from before */
	else if (filep->IOcnt + count >= filep->IOsiz) { //==?
		nbytetomemcpy = IObufspace;
		memcpy(filep->IObuf + filep->IOoffset, inbuf, nbytetomemcpy);
		write(filep->filedesc, filep->IObuf, filep->IOsiz); //myflush(filep);
		nbytetomemcpy = count - nbytetomemcpy;
		inbufoffset += nbytetomemcpy;
		filep->IOoffset = 0;
		filep->IOcnt = 0;
	}

	memcpy(filep->IObuf + filep->IOoffset, inbuf + inbufoffset, nbytetomemcpy);
	filep->IOcnt += nbytetomemcpy;
	filep->IOoffset = filep->IOcnt;

	printf("%d\n", filep->IOcnt);
	printf("%s\n", filep->IObuf);

	/* If we attempt to write less than space is available, simply add it to IObuf */
	return count;
}

int main(int argc, char *argv[]) {
	MYFILE *myfile;
	myfile = myopen("test.txt", O_TRUNC | O_RDWR);
	write(myfile->filedesc, "Calm. Kindness. Kinship. Love […] I've given up all chance at inner peace. I've made my mind a sunless space. I share my dreams with ghosts. I wake up every day to an equation I wrote 15 years ago from which there's only one conclusion: I'm damned for what I do.", 262);
	//mywrite(myfile, "Calm. Kindness. Kinship. Love […] I've given up all chance at inner peace. I've made my mind a sunless space. I share my dreams with ghosts. I wake up every day to an equation I wrote 15 years ago from which there's only one conclusion: I'm damned for what I do.", 262);
	//mywrite(myfile, "I yearned to be a savior against injustice without contemplating the cost...", 76);
	//mywrite(myfile, "May the Force be with you.", 26);
	//mywrite(myfile, "I yearned to be a savior against injustice without contemplating the cost...", 76);
	//mywrite(myfile, "May the Force be with you.", 26);
	//mywrite(myfile, "May the Force be with you.", 26);
	//mywrite(myfile, "May the Force be with you.", 26);
	//mywrite(myfile, "I yearned to be a savior against injustice without contemplating the cost, and by the time I looked down, there was no longer any ground beneath my feet […] And the ego that started this fight will never have a mirror or an audience or the light of gratitude. So what do I sacrifice? Everything!", 295);
	//mywrite(myfile, "I yearned to be a savior against injustice without contemplating the cost, and by the time I looked down, there was no longer any ground beneath my feet […] And the ego that started this fight will never have a mirror or an audience or the light of gratitude. So what do I sacrifice? Everything!", 295);
	//mywrite(myfile, "May the Force be with you.", 26);
	//mywrite(myfile, "May the Force be with you.", 26);
	//mywrite(myfile, "May the Force be with you.", 26);
	//mywrite(myfile, "May the Force be with you.", 26);
	//mywrite(myfile, "May the Force be with you.", 26);
	//mywrite(myfile, "May the Force be with you.", 26);
	
	//printf("%d\n", myfile->IOcnt);
	//FILE *myfile;
	//myfile = fopen("test.txt", "r+");
	//fwrite("Calm. Kindness. Kinship. Love […] I've given up all chance at inner peace. I've made my mind a sunless space. I share my dreams with ghosts. I wake up every day to an equation I wrote 15 years ago from which there's only one conclusion: I'm damned for what I do.", sizeof(char), 100000, myfile);
	//fwrite("I yearned to be a savior against injustice without contemplating the cost, and by the time I looked down, there was no longer any ground beneath my feet […] And the ego that started this fight will never have a mirror or an audience or the light of gratitude. So what do I sacrifice? Everything!", sizeof(char), 106, myfile);

	
}

