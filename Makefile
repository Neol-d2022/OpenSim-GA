CC=gcc

CFLAGS=-Wall -Wextra -g3
LFLAGS=

OBJS=avl.o chrom.o conn.o main.o wnode.o population.o selector.o
DEPS=avl.h chrom.h conn.h wnode.h population.h selector.h
LIBS=

BIN=ga

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS) $(LIBS)

clean:
	rm -f $(OBJS) $(BIN)
