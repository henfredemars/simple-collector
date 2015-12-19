
CPP_FILES = collector.cpp gc_obj.cpp
H_FILES = include/collector.h include/gc_obj.h

all: $(CPP_FILES) $(H_FILES)
	g++ -std=c++11 $(GCC_OPTS) -c $(CPP_FILES)

clean:
	rm -rf *.o
