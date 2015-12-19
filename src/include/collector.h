
//Garbage collector

#ifndef __COLLECTOR_H
#define __COLLECTOR_H

#include <cassert>
#include <unordered_set>
#include <vector>
#include "gc_obj.h"

class Collector {
public:
	Collector();
	~Collector();
	void addRoot(gc_obj* obj);
	void addObject(gc_obj* obj);
	void dropRoot(gc_obj* obj);
	void dropObject(gc_obj* obj);
	int countRoots() const;
	int countAllocs() const;
	int getTotalMonitored() const;
	int getTotalCollected() const;
	int getTotalCollections() const;
	void collect();
	void printStats() const;

	//Forbid copies of the same collector instance
	Collector& operator=(const Collector&) = delete;
	Collector(Collector&&) = delete;
	Collector(const Collector&) = delete;
	Collector& operator=(Collector&&) = delete;
private:
	void clearMarkBits();
	void addAlloc(gc_obj* obj);
	void assertOnHeap(const gc_obj* obj) const;
	std::unordered_set<gc_obj*> root_objs;
	std::unordered_set<gc_obj*> alloc_objs;
	int totalMonitored;
	int totalCollected;
	int totalCollections;
};

#endif /* __COLLECTOR_H */
