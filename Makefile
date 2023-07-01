CC = gcc
SOURCE = cimin.c
OUTPUT = cimin

all:
	$(CC) $(SOURCE) -o $(OUTPUT)

debug:
	$(CC) $(SOURCE) -Wall -Wextra -DDEBUG -o $(OUTPUT)

clean:
	rm -f cimin
	rm -rf cimin.dSYM
