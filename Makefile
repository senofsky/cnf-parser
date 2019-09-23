.PHONY: clean test

default: test

test: cnf.o test_cnf.o
	gcc cnf.c test_cnf.c -o test.elf

clean:
	rm -f test.elf
	rm -f *.o
