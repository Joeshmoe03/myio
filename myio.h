typedef struct {
	int filedesc;
	char *buf;
	int bufsize;
	int bufcount;
	int useroffset;
	int offset;
	//SOMETHING;
} MYFILE;

//_______________________________________________________________________________________

/* Function prototype for myopen */
MYFILE* myopen(const char* path, int flags);

void mywrite(MYFILE* filep, char *buf, int count);

int myclose();

int myread();

int myseek();

int myflush();
