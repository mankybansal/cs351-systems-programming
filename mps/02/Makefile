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
	head -10 p096_sudoku.txt | ./sudoku

diff: sudoku
	./sudoku < p096_sudoku.txt | diff - solutions.txt 

clean:
	rm -f $(OBJS) sudoku
