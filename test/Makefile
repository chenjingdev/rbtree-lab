.PHONY: test test2 test3

CFLAGS=-I ../src -Wall -g -DSENTINEL

test: test-rbtree
	./test-rbtree
	valgrind ./test-rbtree

test2: test-rbtree2
	./test-rbtree2
	valgrind ./test-rbtree2

test3: test-rbtree3
	./test-rbtree3
	valgrind ./test-rbtree3

test-rbtree: test-rbtree.o ../src/rbtree.o
	$(CC) $(CFLAGS) -o test-rbtree test-rbtree.o ../src/rbtree.o

test-rbtree2: test-rbtree.o ../src/rbtree2.o
	$(CC) $(CFLAGS) -o test-rbtree2 test-rbtree.o ../src/rbtree2.o

test-rbtree3: test-rbtree.o ../src/rbtree3.o
	$(CC) $(CFLAGS) -o test-rbtree3 test-rbtree.o ../src/rbtree3.o

../src/rbtree.o:
	$(MAKE) -C ../src rbtree.o

../src/rbtree2.o:
	$(MAKE) -C ../src rbtree2.o

../src/rbtree3.o:
	$(MAKE) -C ../src rbtree3.o

clean:
	rm -f test-rbtree test-rbtree2 test-rbtree3 *.o
