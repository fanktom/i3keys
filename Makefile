CFLAGS=-I. -Wall
DEPS = i3keys.h
OBJS = main.o
LIBS=-lm

all: i3keys

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

i3keys: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f i3keys *.o

.PHONY: clean
