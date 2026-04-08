CC	= /opt/gcc/14.2.0/bin/gcc
CFLAGS	= -g -O3 -pedantic -Werror -mcpu=power8 -fopenmp -pthread -c23
OBJS	= matching.o main.o error.o tbr.o timebase.o
LDFLAGS	= -fopenmp -pthread -lm -static -c23
OUT	= a.out
NOTE	= note
FILE	= matching

main: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(CFLAGS) -lm

compile: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS)

execute: compile
	./a.out

clean:
	rm -f *.o a.out
