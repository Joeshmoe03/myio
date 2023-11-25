
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "myio.h"

int main(int argc, char *argv[]) {
	
	/* This silly little program rewrites parts of the Declaration of Independence with certain passages from
	* Palpatine's announcment of the formation of the Galactic Empire. To see the actual result, take a look at
	* the file "writefile0.txt" after compiling and running. 
	* IMPORTANT! TO CHECK CORRECTNESS:
	* 	$ vi writefile0.txt */

	 /* DIfile and other DI-stuff are for our file containing the Declaration of Independence */
	 int DIcnt = 0;
	 char DIbuf[50000];
	 MYFILE* DIfile;
	 DIfile = myopen("./testfiles/readfile0.txt", O_RDONLY);

	 /* SWfile and associated SW-stuff are for our file containing Palpatine's proclamation of the Galactic Empire */
	 int SWcnt = 0;
	 char SWbuf[50000];
	 MYFILE* SWfile;
	 SWfile = myopen("./testfiles/readfile1.txt", O_RDONLY);

	 /* Open up the file we plan to mostly write to but will read a bit */
	 int WRcnt = 0;
	 MYFILE* WRfile;
	 WRfile = myopen("writefile0.txt", O_CREAT | O_TRUNC | O_RDWR);

	 /* Read the first sentence and title from the declaration of independence */
	 DIcnt += myread(DIfile, DIbuf + DIcnt, 25);
	 DIcnt += myread(DIfile, DIbuf + DIcnt, 0);
	 DIcnt += myread(DIfile, DIbuf + DIcnt, 250);
	 DIcnt += myread(DIfile, DIbuf + DIcnt, 130);
	 DIcnt += myread(DIfile, DIbuf + DIcnt, 22);
	 DIcnt += myread(DIfile, DIbuf + DIcnt, 76);

	 /* We write the first part	of what was read to the DIfile */
	 WRcnt += mywrite(WRfile, DIbuf + WRcnt, DIcnt - 34);
	 WRcnt += mywrite(WRfile, DIbuf + WRcnt, 2);
	 WRcnt += mywrite(WRfile, DIbuf + WRcnt, 32);
	 DIcnt = 0;

	 /* We read the first part of the emperor's speech */
	 SWcnt += myread(SWfile, SWbuf + SWcnt, 0);
	 SWcnt += myread(SWfile, SWbuf + SWcnt, 188);
	 SWcnt += myread(SWfile, SWbuf + SWcnt, 12);
	 SWcnt += myread(SWfile, SWbuf + SWcnt, 12);
	 SWcnt += myread(SWfile, SWbuf + SWcnt, 150);
	
	 /* We write Palpatine's speech to file and skip to another interesting paragraph to read */
	 mywrite(WRfile, SWbuf, SWcnt);
	
	 myseek(SWfile, 1463, SEEK_CUR);
	 SWcnt += myread(SWfile, SWbuf + SWcnt, 357);

	 /* Write the new contents of what we just seeked and read */	
	 mywrite(WRfile, SWbuf, SWcnt);
	 SWcnt = 0;

	 /* Now fetch more of the Decl. of Ind. by using Myseek with Seek_set */
	 myseek(DIfile, DIcnt + 842, SEEK_SET);
	 DIcnt += myread(DIfile, DIbuf + DIcnt, 4000);
	 mywrite(WRfile, DIbuf, 627); 

	 /* Close our files */
	 myclose(WRfile);
	 myclose(DIfile);
	 myclose(SWfile);

	/* END OF THE DECLARATION OF INDEPENDENCE TEST */

	/* good open */ //works//
	MYFILE *f1 = myopen("./testfiles/jarjar.txt", O_RDWR);
	//perror("fopen");

	FILE* ffile = fopen("./testfiles/jarjar.txt", "r+");

	/* try to open the same file multiple times */	//works but... unsure if behavior down the line works as intended//
	// MYFILE *myfileonce = myopen("jarjar.txt", O_RDWR);
	// perror("fopen");
	// MYFILE *myfiletwice = myopen("jarjar.txt", O_RDWR);
	// perror("fopen");
 
	/*!!! basic reading !!!*/
	/* All test cases follow assumption that BUFFER_SIZE is 25 */

	// char bufx[1000];

	/* actual: fread() */
	/* Tests Desired Behavior & Return Values */
	/* Test for many consecutive myread */
	//  printf("%ld\n",fread(bufx,1,125,ffile));
	//  printf(bufx, "\n\n");
	//  printf("%ld\n",fread(bufx,1,5,ffile));
	//  printf(bufx, "\n\n");
	//  printf("%ld\n",fread(bufx,1,70,ffile));
	//  printf(bufx, "\n\n");
	//  printf("%ld\n",fread(bufx,1,0,ffile));
	//  printf(bufx, "\n\n");
	//  printf("%ld\n",fread(bufx,1,11,ffile));
	//  printf(bufx, "\n\n");
	//  printf("%ld\n",fread(bufx,1,27,ffile));
	//  printf(bufx, "\n\n");
	//  printf("%ld\n",fread(bufx,1,1,ffile));
	//  printf(bufx, "\n\n");
	//  printf("%ld\n",fread(bufx,1,1,ffile));
	//  printf(bufx, "\n\n");
	//  printf("%ld\n",fread(bufx,1,150,ffile));


	/* Edge Cases */
	//TODO: stuff in buffer and try to read nothing-ness (NOT 0, but rather emptyness) and re-implement buffer (Case 2a(ii))
	//memset??
	//put 5 bytes, try to read 30 bytes of nothingness, and then read(5), going to be reading the 1st five bytes we put in, but rlly only want to read nothingness

	/* ours: myread() */
	/* myread structure */
	/* Case 0: Reading nothing */
	/* Case 1: Buffer is "empty" */
		/* Case 1a: Outside limits of our buffer */ //works//
		/* Case 1b: Within limits of our buffer */
	/* Case 2: Buffer is NOT "empty" */
		/* Case 2a: User asks to read more bytes (or same amount) than the remaining bytes in our IObuffer */
			/* Case 2a(i): After giving what's left in our buffer, what we still need to give them is outside limits of our buffer */
			/* Case 2a(ii): After giving what's left in our buffer,  what we still need to give them is within limis of our buffer */
		/* Case 2b: User attempts to read an amount of bytes that our IObuffer can handle with remaining space: Default Buffering */

	/* Testing following cases: Case 0, Case 1a, Case 1b, Case 2a(i), Case 2a(ii), Case 2b as these are the "functional" cases */
	/* Tests Desired Behavior & Return Values */
	/* Test for many consecutive myread */
	// char bufy[10000];
	// printf("%d\n",myread((f1), bufy, 125)); /* Case 1a */ //works//
	// printf(bufy, "\n\n");
	// printf("%d\n",myread((f1), bufy, 5)); //Case 1b //works//
	// printf(bufy, "\n\n");
	// printf("%d\n",myread((f1), bufy, 70)); //Case 2a(i) //after this the buffer will be "empty" //works//
	// printf(bufy, "\n\n");
	// printf("%d\n",myread((f1), bufy, 0)); //Case 0 //works//
	// printf(bufy, "\n\n");
	// printf("%d\n",myread((f1), bufy, 11)); //Case 1b //works//
	// printf(bufy, "\n\n");
	// printf("%d\n",myread((f1), bufy, 27)); //Case 2a(ii) //works//
	// printf(bufy, "\n\n");
	// printf("%d\n",myread((f1), bufy, 1)); //Case 2b //works//
	// printf(bufy, "\n\n");
	// printf("%d\n",myread((f1), bufy, 1)); //Case 2b //works//
	// printf(bufy, "\n\n");
	// printf("%d\n",myread((f1), bufy, 150));

	/* Edge Cases */
	//TODO: stuff in buffer and try to read nothing-ness (NOT 0, but rather emptyness) and re-implement buffer (Case 2a(ii))
	//memset??
	//put 5 bytes, try to read 30 bytes of nothingness, and then read(5), going to be reading the 1st five bytes we put in, but rlly only want to read nothingness

	/*!!! basic writing !!!*/
	/* actual: fwrite() */
	/* Test for many consecutive mywrite */

	// FILE *writefile1;
	// writefile1 = fopen("./testfiles/write0.txt", "w");
	// fwrite("I yearned to be a savior against injustice without contemplating the cost...", 1, 76, writefile1);
	// fwrite("May the Fourth", 1, 14, writefile1);
	// fwrite("I yearned to be a savior against the world", 1, 42, writefile1);
	// fwrite("", 1, 0, writefile1);
	// fwrite("I am YOUR FATHER!", 1, 17, writefile1);
	// fwrite("This isss 19 bytes?", 1, 19, writefile1);
	// fwrite("lil kid", 1, 7, writefile1);
	// fwrite("ahh", 1, 3, writefile1);

	/* Edge cases */
	// fwrite("this is just wrong", 1, 27, writefile1);
	// fwrite("right?", 1, 2, writefile1);

	/* ours: mywrite() */
	/* mywrite() conceptual structure */
	/* Case 0: Writing nothing */
	/* Case 1: Buffer is "empty" */
		/* Case 1a: Outside limits of our buffer */
		/* Case 1b: Within limits of our buffer: Default Buffering */
	/* Case 2: Buffer is NOT "empty" */
		/* Case 2a: User asks to write more bytes (or same amount) than the remaining bytes in our IObuffer */
			/* Case 2a(i):	After flushing, what we still need to give them is outside limits of our buffer */
			/* Case 2a(ii): After flushing, what we still need to give them is within limis of our buffer */
		/* Case 2b: User attempts to write an amount of bytes that our IObuffer can handle with remaining space */

	/* Test for many consecutive mywrite */
	MYFILE *writefile0;
	writefile0 = myopen("./testfiles/write0.txt", O_CREAT | O_TRUNC | O_RDWR);
	mywrite(writefile0, "I yearned to be a savior against injustice without contemplating the cost...", 76); /* Case 1a */
	mywrite(writefile0, "May the Fourth", 14); /* Case 1b */
	mywrite(writefile0, "I yearned to be a savior against the world", 42); /* Case 2a(i) */ // buffer will be "empty" b/cuz flush "resets" it
	mywrite(writefile0, "", 0); /* Case 0 */ //causes off by one!!!

	///*ONLY running from here down is also a problem!!!*/
	mywrite(writefile0, "I am YOUR FATHER!", 17); /* Case 1b */
	mywrite(writefile0, "This isss 19 bytes?", 19); /* Case 2a(ii) */
	mywrite(writefile0, "lil kid", 7); /* Case 2b */
	mywrite(writefile0, "ahh", 3); /* Case 2b */
	myflush(writefile0);
	myclose(writefile0);

	///* Edge Cases */
	////mywrite(writefile0, "this is just wrong", 27); /* count > len(input) */ //works
	////perror("fwrite");
	////mywrite(writefile0, "right?", 2); /* count < len(input) */ //works
	////myflush(writefile0);

	///*!!! basic seeking !!!*/

	///* actual: fseek() */
	//// fwrite("chant",1,5,ffile);
	//// fseek(ffile, 5, SEEK_CUR);
	//// fread(buf, 5,1,ffile);
	//// printf(buf, "\n\n");
	//// fseek(ffile, 50, SEEK_SET);
	//// fseek(ffile, -10, SEEK_CUR);
	//// fread(buf, 5,1,ffile);
	//// fseek(ffile, 10, SEEK_CUR);
	//// fwrite("chant",1,5,ffile);
	//// printf(buf, "\n\n");

	///* ours: myseek() */ //works//
	//mywrite(f1,"chant", 5);
	//myseek(f1, 5, SEEK_CUR);
	//myread(f1, buf, 5);
	////printf(buf, "\n\n");
	//myseek(f1, 50, SEEK_SET);
	//myseek(f1, -10, SEEK_CUR);
	//myread(f1, buf, 5);
	//myseek(f1, 10, SEEK_CUR);
	//mywrite(f1,"chant", 5);
	//myflush(f1);
	////printf(buf, "\n\n");

	/* !!! intermingling read/write !!! */
	/* NOTE: TO VERIFY INTERMINGLING CORRECTNESS RUN: 
 	* 	$ make clean 
 	* 	$ make
 	* 	$ ./myio
 	* 	$ diff ./testfiles/rdwrinter0.txt ./testfiles/rdwrinter1.txt */
	MYFILE* rdwr0 = myopen("./testfiles/rdwrinter0.txt", O_RDWR);
	FILE* rdwr1 = fopen("./testfiles/rdwrinter1.txt", "r+");
	char bufig[1000];
	char bufig2[1000];

	/* OURS */
	myread((rdwr0), bufig2, 125);
	mywrite(rdwr0, "I am YOUR FATHER!", 17);
	myread((rdwr0), bufig2, 2);
	myread((rdwr0), bufig2, 0);
	mywrite(rdwr0, "This isss 19 bytes?", 19);
	myread(rdwr0, bufig2, 4);
	mywrite(rdwr0, "Esto essssssss 26 bytes?", 24);	
	myread(rdwr0, bufig2, 45);
	
	/* ACTUAL */
	fread(bufig,1,125,rdwr1);
	fwrite("I am YOUR FATHER!", 1, 17, rdwr1);
	fread(bufig,1,2,rdwr1);
	fread(bufig,1,0,rdwr1);
	fwrite("This isss 19 bytes?", 1, 19, rdwr1);
	fread(bufig,1,4,rdwr1);
    fwrite("Esto essssssss 26 bytes?", 1, 24, rdwr1);
    fread(bufig, 1,45,rdwr1);	

	/* !!! basic closing !!! */
	/* actual: fclose() */
	fclose(ffile);
	fclose(rdwr1);

	/* our: myclose() */
	myclose(f1);
	myclose(rdwr0);
}
