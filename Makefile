all: testprog.out

testprog.out: main.o sha256.o
	gcc -g -o testprog.out main.o sha256.o

main.o: main.c sha256.h
	gcc -c main.c

sha256.o: sha256.c sha256.h
	gcc -c sha256.c

clean:
	rm *.o *.out *.d
