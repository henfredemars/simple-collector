
GCC_OPTS:= -std=c++11 $(GCC_OPTS)

test.bin: test.o binaryTreeNode.o
	g++ $(GCC_OPTS) -o test.bin test.o binaryTreeNode.o ../src/*.o

test.o: test.cpp
	g++ $(GCC_OPTS) -c test.cpp

binaryTreeNode.o: binaryTreeNode.cpp
	g++ $(GCC_OPTS) -c binaryTreeNode.cpp

clean:
	rm -f *.o *.bin
