CC      = gcc
CFLAGS  = -g -Wall
SRCS    = hashtable.c main.c
OBJS    = $(SRCS:.c=.o)
SED     = sed

all: hashtable

hashtable: $(OBJS)
	$(CC) $(CFLAGS) -o hashtable $(OBJS)

demo: hashtable-demo.o main.o
	$(CC) $(CFLAGS) -o hashtable-demo hashtable-demo.o main.o

test01: hashtable
	@./hashtable trace01.txt

test02: hashtable
	@./hashtable trace02.txt

test03: hashtable
	@./hashtable trace03.txt

test04: hashtable
	@./hashtable trace04.txt

test05: hashtable
	@./hashtable trace05.txt

test06: hashtable
	@./hashtable trace06.txt

diff01: hashtable
	@./hashtable trace01.txt | diff - rtrace01.txt

diff02: hashtable
	@./hashtable trace02.txt | diff - rtrace02.txt

diff03: hashtable
	@./hashtable trace03.txt | diff - rtrace03.txt

diff04: hashtable
	@./hashtable trace04.txt | diff - rtrace04.txt

diff05: hashtable
	@./hashtable trace05.txt | diff - rtrace05.txt

diff06: hashtable
	@./hashtable trace06.txt | diff - rtrace06.txt

leakcheck: hashtable
	@valgrind --leak-check=full ./hashtable trace06.txt

clean:
	rm -f $(OBJS) hashtable hashtable-demo hashtable-demo.o
