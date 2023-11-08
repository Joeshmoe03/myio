/*
 * mytestmyio.c
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "myio.h"

int main(int argc, char *argv[])
{
    
    /* myopen */


    /* bad flags & combinations */


    /* bad file names */

    /* flag for file whose permissions don't align? */


    
    /* myread */
    /*basic reading (sub cases of: outside limits of buffer, within limits of our buffer, try to go beyond limits of our buffer (with smth not too big)) */
    

    //BAD when we have smth in the buffer and then read smth > BUFFER SIZE

    /* mywrite */ 
    /* basic write (sub cases of: idk) */
    
    /* flush */

    /* close */    
    
    //printf("works\n");
   //printf("%d\n", myopen("hello.txt", O_RDWR | O_RDONLY | O_WRONLY | O_CREAT | O_TRUNC));
    //perror("open");
    //printf("Worked?\n");
    
    
    myopen("jarjar.txt", O_RDWR);
    FILE *ffile = fopen("jarjar.txt", "r");
    MYFILE *f1 = myopen("jarjar.txt", O_RDWR);
    // //int fd1 = open("hello.txt", O_RDWR);
    // perror("open");

    char buf[100];
//     //char buf2[10240];

//     printf("%d",f1->filedesc);
//     //read(fd1, buf, 10);
//     //read((f1->filedesc), buf, 10);

    myread((f1), buf, 5); //6 memory errors
    printf(buf, "\n\n"); 
    myread((f1), buf, 70); //2 memory erros //IT'S WRONG
    printf(buf, "\n\n"); 
    myread((f1), buf, 0); //6 memory errors
    printf(buf, "\n\n"); 
    myread((f1), buf, 25); //2 memory erros //IT'S WRONG
    printf(buf, "\n\n"); 
    myread((f1), buf, 26); //6 memory errors
    printf(buf, "\n\n"); 
    myread((f1), buf, 24); //2 memory erros //IT'S WRONG
    printf(buf, "\n\n"); 
    myread((f1), buf, 1); //6 memory errors
    printf(buf, "\n\n"); 
    myread((f1), buf, 1);
    printf(buf, "\n\n"); 
    myread((f1), buf, 99);
    printf(buf, "\n\n"); 
    myread((f1), buf, 101);
    printf(buf, "\n\n"); 
    myread((f1), buf, 17); //good up to here (begin jumping ahead in both CASEs); //worked with a call of 5 next
    printf(buf, "\n\n"); 
    myread((f1), buf, 12); 
    printf(buf, "\n\n"); 
    myread((f1), buf, 11);
    printf(buf, "\n\n"); 
    myread((f1), buf, 27);



    // fread(buf,1,5,ffile);
    // printf(buf, "\n\n"); 
    // fread(buf,1,70,ffile);
    // printf(buf, "\n\n"); 
    // fread(buf,1,25,ffile);
    // printf(buf, "\n\n"); 
    // fread(buf,1,26,ffile);
    // printf(buf, "\n\n"); 
    // fread(buf,1,24,ffile);
    // printf(buf, "\n\n"); 
    // fread(buf,1,1,ffile);
    // printf(buf, "\n\n"); 
    // fread(buf,1,1,ffile);
    // printf(buf, "\n\n"); 
    // fread(buf,1,99,ffile);
    // printf(buf, "\n\n"); 
    // fread(buf,1,101,ffile);
    // printf(buf, "\n\n"); 
    // fread(buf,1,17,ffile);
    // printf(buf, "\n\n"); 
    // fread(buf,1,12,ffile);
    // printf(buf, "\n\n"); 
    // fread(buf,1,11,ffile);
    // printf(buf, "\n\n"); 
    // fread(buf,1,27,ffile);


    // read((f1->filedesc), buf, 5); //6 memory errors
    // printf(buf, "\n\n"); 
    // read((f1->filedesc), buf, 70); //2 memory erros //IT'S WRONG
    // printf(buf, "\n\n"); 
    // read((f1->filedesc), buf, 0); //6 memory errors
    // printf(buf, "\n\n"); 
    // read((f1->filedesc), buf, 25); //2 memory erros //IT'S WRONG
    // printf(buf, "\n\n"); 
    // read((f1->filedesc), buf, 26); //6 memory errors
    // printf(buf, "\n\n"); 
    // read((f1->filedesc), buf, 24); //2 memory erros //IT'S WRONG
    // printf(buf, "\n\n"); 
    // read((f1->filedesc), buf, 1); //6 memory errors
    // printf(buf, "\n\n"); 
    // read((f1->filedesc), buf, 1);
    // printf(buf, "\n\n"); 
    // read((f1->filedesc), buf, 99);
    // printf(buf, "\n\n"); 
    // read((f1->filedesc), buf, 101);
    // printf(buf, "\n\n"); 
    // read((f1->filedesc), buf, 17); //good up to here (begin jumping ahead in both CASEs); //worked with a call of 5 next
    // printf(buf, "\n\n"); 
    // read((f1->filedesc), buf, 12); 
    // printf(buf, "\n\n"); 
    // read((f1->filedesc), buf, 11);
    // printf(buf, "\n\n"); 
    // read((f1->filedesc), buf, 27);


    //myseek(f1, 105, SEEK_SET);
    //myseek(f1, 50, SEEK_CUR);
    // myread(f1, buf, 5);

    //ACTUALLY SEEMS TO BE WORKING FINE EXCEPT FOR A FEW TINY EDGE CASES LIKE SPACES, ETC.
    // lseek(f1->filedesc, 5, SEEK_SET);
    // // read(f1->filedesc, buf, 64);
    // read(f1->filedesc, buf, 5);
    // read(f1->filedesc, buf, 70);
    // read(f1->filedesc, buf, 20);
    // read(f1->filedesc, buf, 5);
    // read(f1->filedesc, buf, 25);
    //fread()
    // read(f1->filedesc, buf, 5);
    // read(f1->filedesc, buf, 5);
    // read(f1->filedesc, buf, 10); 
    // read(f1->filedesc, buf, 1);   
    // read(f1->filedesc, buf, 1);     
    // read(f1->filedesc, buf, 10);  
    

    // lseek(f1->filedesc, 50, SEEK_CUR);
    // lseek(f1->filedesc, 50, SEEK_CUR);
    // read(f1->filedesc, buf, 5);

    printf(buf, "\n\n"); 

    myclose(f1);
//     perror("read");
// //     // // int try1 = open("hello.txt", O_RDWR);
// //     // // perror("open");

// //     // // read(try1, buf2, 10240);
// //     // // perror("read");

   //printf(buf, "\n\n"); //causes memory error (weird); our myread function seems to be doing fine error wise
// //     // printf(buf2);

}

