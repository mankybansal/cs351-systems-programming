CC = gcc
CFLAGS = -Wall -g
FILES = ./cachesim

all: ./cachesim

test01: all
	./cachesim 8 1 1 < trace01.txt 
	./cachesim 8 4 1 < trace01.txt
	./cachesim 16 1 1 < trace01.txt
	./cachesim 16 1 16 < trace01.txt
	./cachesim 16 1 32 < trace01.txt

test02: all
	./cachesim 16 1 1 < trace02.txt 
	./cachesim 16 1 8 < trace02.txt
	./cachesim 16 1 16 < trace02.txt
	./cachesim 1024 1 1 < trace02.txt
	./cachesim 128 1 8 < trace02.txt

test03: all
	bunzip2 -c trace03.txt.bz2 | ./cachesim 128 1 8
	bunzip2 -c trace03.txt.bz2 | ./cachesim 128 8 16
	bunzip2 -c trace03.txt.bz2 | ./cachesim 512 8 32
	bunzip2 -c trace03.txt.bz2 | ./cachesim 1024 8 64

clean:
	rm -rf $(FILES) *.o *~ *.dSYM


