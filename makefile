CFLAGS=-ggdb -pedantic -Wall -Wextra

main: main.c
	$(CC) $(CFLAGS) $^ -o $@
