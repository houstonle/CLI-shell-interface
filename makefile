CC = g++
CFLAGS = -Wall -std=c++11

all: shell379

shell379: shell.o commands.o utility.o
	$(CC) $(CFLAGS) shell.o commands.o utility.o -o shell379

shell.o: shell.cpp
	$(CC) $(CFLAGS) -c shell.cpp -o shell.o

commands.o: commands.cpp
	$(CC) $(CFLAGS) -c commands.cpp -o commands.o

utility.o: utility.cpp
	$(CC) $(CFLAGS) -c utility.cpp -o utility.o

clean:
	rm -f *~out.* *.o
