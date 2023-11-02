/*
 * mytestmyio.c
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "myio.h"

int main(int argc, char *argv[])
{
    printf("works\n");
   //printf("%d\n", myopen("hello.txt", O_RDWR | O_RDONLY | O_WRONLY | O_CREAT | O_TRUNC));
    //perror("open");
    //printf("Worked?\n");
    
    
    //myopen("hello.txt", O_RDWR);
    MYFILE *f1 = myopen("hello.txt", O_RDWR);
    //int fd1 = open("hello.txt", O_RDWR);
    perror("open");

    char buf[100];
//     //char buf2[10240];

//     printf("%d",f1->filedesc);
//     //read(fd1, buf, 10);
//     //read((f1->filedesc), buf, 10);

    // myread((f1), buf, 64); //6 memory errors
    // myread((f1), buf, 5); //2 memory erros
    // myread((f1), buf, 5);
    // myread((f1), buf, 5);
    // myread((f1), buf, 5);
    // myread((f1), buf, 10); //good up to here (begin jumping ahead in both CASEs); //worked with a call of 5 next
    // myread((f1), buf, 1); 
    // myread((f1), buf, 1);
    // myread((f1), buf, 10);


    myseek(f1, 105, SEEK_SET);
    //myseek(f1, 50, SEEK_CUR);
    // myread(f1, buf, 5);

    //ACTUALLY SEEMS TO BE WORKING FINE EXCEPT FOR A FEW TINY EDGE CASES LIKE SPACES, ETC.
    // lseek(f1->filedesc, 5, SEEK_SET);
    // // read(f1->filedesc, buf, 64);
    // read(f1->filedesc, buf, 5);
    // read(f1->filedesc, buf, 5);
    // read(f1->filedesc, buf, 5);
    // read(f1->filedesc, buf, 5);
    // read(f1->filedesc, buf, 10); 
    // read(f1->filedesc, buf, 1);   
    // read(f1->filedesc, buf, 1);     
    // read(f1->filedesc, buf, 10);  
    

    // lseek(f1->filedesc, 50, SEEK_CUR);
    // lseek(f1->filedesc, 50, SEEK_CUR);
    // read(f1->filedesc, buf, 5);


    myclose(f1);
//     perror("read");
// //     // // int try1 = open("hello.txt", O_RDWR);
// //     // // perror("open");

// //     // // read(try1, buf2, 10240);
// //     // // perror("read");

    printf(buf, "\n\n"); //causes memory error (weird); our myread function seems to be doing fine error wise
// //     // printf(buf2);

}

