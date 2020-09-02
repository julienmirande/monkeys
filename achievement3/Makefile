CC = gcc
CFLAGS= -Wall -Wextra -std=c99
OBJS= queue.o cell.o monkeys.o
DIR=-I. -I../ -I./src/ -I./tests

all: project

project: $(OBJS) main.o
	$(CC) $^ -o $@

monkeys.o: src/monkeys.c src/monkeys.h
	$(CC) $(CFLAGS) -c $<

cell.o: src/cell.c src/cell.h
	$(CC) $(CFLAGS) -c $<

queue.o: src/queue.c src/queue.h
	$(CC) $(CFLAGS) -c $<

main.o: src/main.c
	$(CC) $(CFLAGS) -c $<

test: testexec

testexec: $(OBJS) testQueue.o testMonkeys.o  testMain.o
	$(CC) $^ $(DIR) -o $@


testQueue.o: tests/testQueue.c
	$(CC) $(CFLAGS) $(DIR) -c $<

testMonkeys.o: tests/testMonkeys.c
	$(CC) $(CFLAGS) $(DIR) -c $<

testMain.o: tests/testMain.c
	$(CC) $(CFLAGS) $(DIR) -c $<

clean:
	$(RM) project testexec *[#~]

cleano:
	$(RM) project testexec *.o


.PHONY: all clean cleano
