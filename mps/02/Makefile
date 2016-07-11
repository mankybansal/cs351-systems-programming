CC      = gcc
CFLAGS  = -g -Wall -O2
SRCS    = hashtable.c main.c
OBJS    = $(SRCS:.c=.o)
SED     = sed

all: hashtable

hashtable: $(OBJS)
	$(CC) $(CFLAGS) -o hashtable $(OBJS)

demo: hashtable-demo.o main.o
	$(CC) $(CFLAGS) -o hashtable-demo hashtable-demo.o main.o

stress: hashtable
	@($(SED) -e 's/.*/& \U&/' < /usr/share/dict/words && echo && cat ) \
		| ./hashtable 100000

clean:
	rm -f $(OBJS) hashtable hashtable-demo hashtable-demo.o
