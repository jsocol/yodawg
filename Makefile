CFLAGS=-Wall -g

all: yodawg.o
	$(CC) yodawg.o main.c -o yodawg

clean:
	rm -rf yodawg *.o
