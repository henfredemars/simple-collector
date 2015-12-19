
export GCC_OPTS = -std=c++11 -flto -g -Wall -march=native

all:
	make -C src all

test:
	make -C src all
	make -C test test.bin

clean:
	make -C src clean
	make -C test clean

.PHONY: all
.PHONY: test
