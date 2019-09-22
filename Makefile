.PHONY: clean test

default: test

test: cnf.o test_cnf.o
	gcc cnf.c test_cnf.c -o test

clean:
	rm -f test
	rm -f *.o
