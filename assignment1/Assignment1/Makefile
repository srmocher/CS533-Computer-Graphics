PROG = assignment1

CC = g++
CFLAGS = -g -Iinclude
LIBS = -lglfw3 -framework OpenGL -framework cocoa -framework IOKit -framework CoreVideo
OBJS = $(PROG).o LoadShaders.o gl3w.o

all: $(PROG)

gl3w.o: gl3w.c
	gcc -c $(CFLAGS) -o $@ $<

.cpp.o:
	$(CC) -c $(CFLAGS) -o $@ $<

$(PROG): $(OBJS)
	$(CC) -g -o $@ $^ $(LIBS)

clean:
	rm -f *.o $(PROG)