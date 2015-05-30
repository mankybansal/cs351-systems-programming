CC      = gcc
CFLAGS  = -g -Wall
SRCS    = sudoku.c main.c
OBJS    = $(SRCS:.c=.o) 

all: sudoku

sudoku: $(OBJS)
	$(CC) $(CFLAGS) -o sudoku $(OBJS)

test-all: sudoku
	./sudoku < p096_sudoku.txt

test-one: sudoku
	/usr/bin/head -10 p096_sudoku.txt | ./sudoku

test-hard: sudoku
	./sudoku < top95.txt

diff: sudoku
	./sudoku < p096_sudoku.txt | /usr/bin/diff - solutions.txt

leaktest: sudoku
	/usr/bin/valgrind ./sudoku < p096_sudoku.txt

clean:
	rm -f $(OBJS) sudoku
