CC=gcc

CFLAGS=-Wall -Wextra -g3
LFLAGS=

OBJS=avl.o chrom.o conn.o input.o main.o wnode.o population.o selector.o simulation.o
DEPS=avl.h chrom.h conn.h input.h wnode.h population.h selector.h simulation.h
LIBS=-lm

BIN=ga

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS) $(LIBS)

clean:
	rm -f $(OBJS) $(BIN)
