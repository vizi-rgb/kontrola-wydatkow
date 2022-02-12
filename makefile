CFLAGS=-ggdb -pedantic -Wall -Wextra

main: main.o date.o money.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) $^ -c -o $@ 

date.o: date.c
	$(CC) $(CFLAGS) $^ -c -o $@

money.o: money.c
	$(CC) $(CFLAGS) $^ -c -o $@


.PHONY=clean

clean:
	rm *.o main 

