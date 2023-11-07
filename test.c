
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

int main(int argc, char *argv[]) {
	MYFILE *writefile;
	MYFILE *readfile;
	writefile = myopen("test.txt", O_CREAT | O_TRUNC | O_RDWR);
	readfile = myopen()
	mywrite(writefile, "Calm. Kindness. Kinship. Love […] I've given up all chance at inner peace. I've made my mind a sunless space. I share my dreams with ghosts. I wake up every day to an equation I wrote 15 years ago from which there's only one conclusion: I'm damned for what I do.", 264);
	mywrite(writefile, "I yearned to be a savior against injustice without contemplating the cost...", 76);
	mywrite(writefile, "May the Force be with you.", 26);
	mywrite(writefile, "I yearned to be a savior against injustice without contemplating the cost...", 76);
	mywrite(myfile, "May the Force be with you.", 26);
	mywrite(myfile, "May the Force be with you.", 26);
	mywrite(myfile, "May the Force be with you.", 26);
	mywrite(myfile, "I yearned to be a savior against injustice without contemplating the cost, and by the time I looked down, there was no longer any ground beneath my feet […] And the ego that started this fight will never have a mirror or an audience or the light of gratitude. So what do I sacrifice? Everything!", 297);
	mywrite(myfile, "I yearned to be a savior against injustice without contemplating the cost, and by the time I looked down, there was no longer any ground beneath my feet […] And the ego that started this fight will never have a mirror or an audience or the light of gratitude. So what do I sacrifice? Everything!", 297);
	mywrite(myfile, "May the Force be with you.", 26);
	mywrite(myfile, "May the Force be with you.", 26);
	mywrite(myfile, "May the Force be with you.", 26);
	mywrite(myfile, "May the Force be with you.", 26);
	mywrite(myfile, "May the Force be with you.", 26);
	mywrite(myfile, "May the Force be with you.", 26);
	myflush(myfile);
	//printf("%d\n", myfile->IOcnt);
	//FILE *myfile;
	//myfile = fopen("test.txt", "r+");
	//fwrite("Calm. Kindness. Kinship. Love […] I've given up all chance at inner peace. I've made my mind a sunless space. I share my dreams with ghosts. I wake up every day to an equation I wrote 15 years ago from which there's only one conclusion: I'm damned for what I do.", 1, 262, myfile);
	//fwrite("Calm. Kindness. Kinship. Love […] I've given up all chance at inner peace. I've made my mind a sunless space. I share my dreams with ghosts. I wake up every day to an equation I wrote 15 years ago from which there's only one conclusion: I'm damned for what I do.", sizeof(char), 100000, myfile);
	//fwrite("I yearned to be a savior against injustice without contemplating the cost, and by the time I looked down, there was no longer any ground beneath my feet […] And the ego that started this fight will never have a mirror or an audience or the light of gratitude. So what do I sacrifice? Everything!", sizeof(char), 106, myfile);

	
}

