CFLAGS=-Wall -g -pedantic

PROGS=myio

myio: myio.c testmyio.c myio.h
	gcc $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f *.o $(PROGS)
