CFLAGS=-Wall -g -pedantic

PROGS=myio test2

myio: myio.c test.c myio.h
	gcc $(CFLAGS) -o $@ $^

test2: myio.c newtest.c myio.h
	gcc $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f *.o $(PROGS)
	rm -f writefile0.txt
	rm -f ./testfiles/testfile.txt
	rm -f ./testfiles/testfile0.txt
	rm -f ./testfiles/rdwrinter0.txt
	rm -f ./testfiles/rdwrinter1.txt
	cp ./testfiles/lotr.txt ./testfiles/testfile0.txt
	cp ./testfiles/lotr.txt ./testfiles/testfile1.txt
	cp ./testfiles/jarjar.txt ./testfiles/rdwrinter0.txt
	cp ./testfiles/jarjar.txt ./testfiles/rdwrinter1.txt
