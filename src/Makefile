.PHONY: clean all

CFLAGS=-Wall -g

all: driver driver2 driver3

driver: driver.o rbtree.o
	gcc $(CFLAGS) -o driver driver.o rbtree.o

driver2: driver.o rbtree2.o
	gcc $(CFLAGS) -o driver2 driver.o rbtree2.o

driver3: driver.o rbtree3.o
	gcc $(CFLAGS) -o driver3 driver.o rbtree3.o

rbtree.o: rbtree.c
	gcc $(CFLAGS) -c rbtree.c

rbtree2.o: rbtree2.c
	gcc $(CFLAGS) -c rbtree2.c

rbtree3.o: rbtree3.c
	gcc $(CFLAGS) -c rbtree3.c

clean:
	rm -f driver driver2 driver3 *.o