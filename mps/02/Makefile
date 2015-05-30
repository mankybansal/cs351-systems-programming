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

clean:
	rm -f $(OBJS) sudoku
