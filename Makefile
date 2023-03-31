CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SOURCE = cimin.c
OUTPUT = cimin

all:
	$(CC) $(SOURCE) $(CFLAGS) -o $(OUTPUT)

debug:
	$(CC) $(SOURCE) $(CFLAGS) -DDEBUG -o $(OUTPUT)

clean:
	rm -f cimin cimin.o