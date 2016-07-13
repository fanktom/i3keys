CFLAGS=-I. -Wall -g
DEPS = i3keys.h
OBJS = main.o
LIBS=-lX11

all: i3keys
	./i3keys

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

i3keys: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f i3keys *.o

.PHONY: clean
