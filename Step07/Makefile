CC = gcc
CFLAGS = -O2 -Wall -I/usr/include/glib-2.0
LDFLAGS = -L/usr/lib -lm

PROGRAM = backpro

OBJS = $(patsubst %.c,%.o,$(wildcard *.c))

all: $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

clean:
	-@rm *~ *.o $(PROGRAM)
