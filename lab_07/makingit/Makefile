CFLAGS=
CC=gcc

SHARED=bSearch.o bubble.o db.o parse.o set.o stack.o

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $*.c

all: database

database: $(SHARED) defs.h parse.h
	$(CC) -o database $(CFLAGS) database.c $(SHARED)

clean: shared/clean database/clean

shared/clean:
	rm *.o

database/clean:
	rm database

run: database
	exec ./database < data	
