all: my_tar

my_tar: my_tar.o
	gcc -o my_tar my_tar.o

my_tar.o: my_tar.c
	gcc -c my_tar.c

clean:
	rm -f my_tar.o
