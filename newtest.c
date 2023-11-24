#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "myio.h"

int main(int argc, char *argv[]) {

	//char temp[1000];
	//MYFILE* tempfile = myopen("almostempty.txt", O_RDWR);
	//printf("I want to read 5: %d\n", myread(tempfile, temp, 5));
	//printf("I want to read 26: %d\n", myread(tempfile, temp, 26));
	//printf("I want to read 91: %d\n", myread(tempfile, temp, 91));
	//printf("I want to read 24: %d\n", myread(tempfile, temp, 24));
	//myclose(tempfile);
	
	char bg[8000];
	FILE* myfile0 = fopen("./testfiles/garbage.txt", "r+");
	fread(bg, 1, 8000, myfile0);
	fclose(myfile0);

	char buf0[10000];
	FILE* myfile2 = fopen("./testfiles/testfile0.txt", "r+");
	fread(buf0, 1, 5000, myfile2);
	fwrite("ppppppppppppp", 1, 5, myfile2);
	fread(buf0, 1, 495, myfile2);
	fread(buf0, 1, 300, myfile2);
	fread(buf0, 1, 4500, myfile2);
	fwrite(bg, 1, 5000, myfile2);
	fread(buf0, 1, 495, myfile2);
	fread(buf0, 1, 305, myfile2);
	fread(buf0, 1, 495, myfile2);
	fread(buf0, 1, 495, myfile2);
	//fread(buf0, 1, 5, myfile2);

	fwrite(bg, 1, 3000, myfile2);

    // fseek
    fseek(myfile2, 50, SEEK_SET);

    // fflush
    fflush(myfile2);

    // fread again
    fread(buf0, 1, 7000, myfile2);

    // fwrite again with count set to 2000
    fwrite(bg, 1, 2000, myfile2);

    // fseek again
    fseek(myfile2, 20, SEEK_SET);

    // fflush again
    fflush(myfile2);	
	fclose(myfile2);

	char bufaux[8000];
	MYFILE* myfileaux = myopen("./testfiles/garbage.txt", O_RDONLY);
	myread(myfileaux, bufaux, 8000);
	myclose(myfileaux);

	char buf1[10000];
	MYFILE* myfile1 = myopen("./testfiles/testfile1.txt", O_RDWR);
	myread(myfile1, buf1, 5000);
	mywrite(myfile1, "ppppppppppppppppp", 5);
	myread(myfile1, buf1, 495); 
	myread(myfile1, buf1, 300);
	myread(myfile1, buf1, 4500);
	mywrite(myfile1, bufaux, 5000);
	myread(myfile1, buf1, 495);
	myread(myfile1, buf1, 305);
	myread(myfile1, buf1, 495);
	myread(myfile1, buf1, 495);

	mywrite(myfile1, bg, 3000);

	myseek(myfile1, 50, SEEK_SET);

	myflush(myfile1);
	
	myread(myfile1, buf1, 7000);

	mywrite(myfile1, bg, 2000);

	myseek(myfile1, 20, SEEK_SET);
	
	myflush(myfile1);
	myclose(myfile1);
}
