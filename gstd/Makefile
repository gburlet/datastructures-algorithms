CC = g++
LDFLAGS = 
CFLAGS = -Wall

main: main.o
	$(CC) $(LDFLAGS) -o $@ $^

main.o: vector.h main.cpp
	$(CC) $(CFLAGS) -c $^

clean:
	rm *.o
