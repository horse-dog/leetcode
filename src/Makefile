CFLAGS=-std=c++11 -g
CC=g++

main: main.cc algorithm.cc
	$(CC) $(CFLAGS) -o $@ $^

rbtest: rbtest.cc
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm main

.PHONY: clean