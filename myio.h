typedef struct {
	int filedesc;
	int *buff;
	int buffsize;
	int useroffset;
	int offset;
	//SOMETHING;
} MYFILE;

//_______________________________________________________________________________________

/* Function prototype for myopen */
MYFILE *myopen(const char* path, int flags);

int myclose();

int myread();

int mywrite();

int myseek();

int myflush();
