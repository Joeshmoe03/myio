/*
 * mytestmyio.c
 */

#include <stdio.h>
#include <fcntl.h>
#include "myio.h"

int
main(int argc, char *argv[])
{
    printf("%d\n", myopen("hello.txt", O_RDWR | O_CREAT));
    perror("open");
   // printf("Worked?\n");
    //myopen("hello.txt", O_RDWR);
    


}
