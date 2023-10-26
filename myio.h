typedef struct {
	int filedesc;
	int buffsize;
	int offset;
	//SOMETHING;
} MYFILE;

//_______________________________________________________________________________________

/* Function prototype for myopen: passed default 0666 */
MYFILE *myopen(const char* path, int flags); //PASS 0666

int myclose();

int myread();

int mywrite();

int myseek();

int myflush();
