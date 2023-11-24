CFLAGS=-Wall -g -pedantic

PROGS=myio test2

myio: myio.c test.c myio.h
	gcc $(CFLAGS) -o $@ $^

test2: myio.c newtest.c myio.h
	gcc $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f *.o $(PROGS)
	rm -f ./testfiles/writefile0.txt
	rm -f ./testfiles/testfile.txt
	rm -f ./testfiles/testfile0.txt
	cp ./testfiles/lotr.txt ./testfiles/testfile0.txt
	cp ./testfiles/lotr.txt ./testfiles/testfile1.txt
