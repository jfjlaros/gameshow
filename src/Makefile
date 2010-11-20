CC = i586-mingw32msvc-cc
CFLAGS = -O3 -Wall
LFLAGS = -lgdi32

EXEC = game.exe
MAIN = game.c
OBJS = 

all: $(EXEC)

.cc.o:
	$(CC) $(CFLAGS) -c $<
	
$(EXEC): $(MAIN) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

release: $(EXEC) clean
	strip $(EXEC)

clean:
	rm -f $(OBJS) core

distclean: clean
	rm -f $(EXEC)
