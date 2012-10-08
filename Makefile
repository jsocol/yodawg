CFLAGS=-Wall -g

all: yodawg.o
	$(CC) build/yodawg.o main.c -o yodawg

yodawg.o:
	$(CC) $(CFLAGS) -c src/yodawg.c -o build/yodawg.o

clean:
	rm -rf yodawg build/*
