CFLAGS=-std=c++14 -g
CC=g++

main: main.cc algorithm.cc
	$(CC) $(CFLAGS) -o $@ $^

rbtest: rbtest.cc
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f main *test

.PHONY: clean