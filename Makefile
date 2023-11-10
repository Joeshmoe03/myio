CFLAGS=-Wall -g -pedantic

PROGS=myio

myio: myio.c test.c myio.h
	gcc $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f *.o $(PROGS)
	rm -f writefile0.txt
