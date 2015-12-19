
#include "include/collector.h"

//Collector initialization
Collector::Collector() {
  totalMonitored = 0;
  totalCollected = 0;
  totalCollections = 0;
}

//Destruction of the collector deletes all objects of responsibility
//Destructors of garbage collected objects are not called. Instead, use finalizer
Collector::~Collector() {
  for (auto iter = root_objs.begin(),end = root_objs.end(); iter!=end; iter++) {
    (*iter)->finalize();
    operator delete(*iter);
  }
  for (auto iter = alloc_objs.begin(),end = alloc_objs.end(); iter!=end; iter++) {
    (*iter)->finalize();
    operator delete(*iter);
  }
}

//Add a root object for monitoring
void Collector::addRoot(gc_obj* obj) {
  if (!obj) return;
  assertOnHeap(obj);
  //We shouldnt be asked to add the same root more than once
  assert((int)root_objs.count(obj)==0 && "Interface Error: Object to be added already in root set.");
  root_objs.insert(obj);
  //Check for already monitored object being upgraded to a root
  if ((int)alloc_objs.count(obj)==0) {
    totalMonitored++;
  } else {
    alloc_objs.erase(obj);
  }
  //Search for new managed children to monitor
  const std::unordered_set<gc_obj*>& linked = obj->getManagedChildren();
  for (auto iter=linked.begin(),end=linked.end(); iter!=end; iter++) {
    gc_obj* candidate = *iter;
    if ((int)alloc_objs.count(candidate)==0 && (int)root_objs.count(candidate)==0) {
      addAlloc(candidate);
    }
  }
}

//Add a non-root object to the collector responsibility
void Collector::addObject(gc_obj* obj) {
  if (!obj) return;
  assertOnHeap(obj);
  assert((int)root_objs.count(obj)==0 && "Interface Error: Object already present within the root set");
  assert((int)alloc_objs.count(obj)==0 && "Interface Error: Object is already monitored");
  addAlloc(obj);
}

//Remove item from the root set
//Note that if the object has managed children that are no longer needed by other
//  roots (either directly or indirectly) then the managed children will become
//  elligible for collection at the next collect()
//Once the object is removed, it is no longer the responsibility of the collector
void Collector::dropRoot(gc_obj* obj) {
  assert((int)root_objs.count(obj)==1 && "Interface Error: Object to be dropped not in root set.");
  root_objs.erase(obj);
}

//Remove item from the collector responsiblity
//The item may or may not be a root object
//Note that if the object has managed children that are no longer needed by the
//  roots (either directly or indirectly) then the managed children will become
//  elligible for collection at the next collect()
//Once the object is removed, it is no longer the responsibility of the collector
void Collector::dropObject(gc_obj* obj) {
  root_objs.erase(obj);
  alloc_objs.erase(obj);
}

//Free all objects that are no longer reachable from the root set
void Collector::collect() {
  clearMarkBits();
  std::vector<gc_obj*> reclaim;
  //Initial marking phase
  for (auto iter = root_objs.begin(),end = root_objs.end(); iter!=end; iter++) {
    (*iter)->_setMarkStateRecursive(GC_BLACK_MARK);
  }
  //Schedule unreachable objects for reclaim
  for (auto iter = alloc_objs.begin(),end = alloc_objs.end(); iter!=end; iter++) {
    gc_obj* candidate = *iter;
    if (candidate->_getMarkState()==GC_WHITE_MARK) {
      reclaim.push_back(candidate);
    }
  }
  //Reclam garbage without calling destructors
  for (auto iter = reclaim.begin(),end = reclaim.end(); iter!=end; iter++) {
    gc_obj* candidate = *iter;
    candidate->finalize();
    alloc_objs.erase(candidate);
    operator delete(candidate);
    totalCollected++;
  }
  totalCollections++;
}

int Collector::countRoots() const {
  return (int)root_objs.size();
}

int Collector::countAllocs() const {
  return (int)alloc_objs.size();
}

int Collector::getTotalMonitored() const {
  return totalMonitored;
}

int Collector::getTotalCollected() const {
  return totalCollected;
}

int Collector::getTotalCollections() const {
  return totalCollections;
}

void Collector::printStats() const {
  int rootSize = countRoots();
  int allocSize = countAllocs();
  printf("Collector Statistics\n================\n\n");
  printf("Current Objects Monitored: %d\n",rootSize+allocSize);
  printf("Root Set Size: %d\n\n",rootSize);
  printf("Lifetime Objects Monitored: %d\n",totalMonitored);
  printf("Lifetime Objects Collected: %d\n",totalCollected);
  printf("Lifetime Number of Collections: %d\n",totalCollections);
}

//Clear the mark state in preparation for a collection
void Collector::clearMarkBits() {
  for (auto iter = root_objs.begin(),end = root_objs.end(); iter!=end; iter++) {
    (*iter)->_setLocalMarkState(GC_WHITE_MARK);
  }
  for (auto iter = alloc_objs.begin(),end = alloc_objs.end(); iter!=end; iter++) {
    (*iter)->_setLocalMarkState(GC_WHITE_MARK);
  }
}

//Private method to add non-root objects and their non-root children to the collector responsibility
void Collector::addAlloc(gc_obj* obj) {
  assert((int)alloc_objs.count(obj)==0 && "INTERNAL ERROR! Object already marked as allocated!");
  assert((int)root_objs.count(obj)==0 && "INTERNAL ERROR! Object already contained within the root set!");
  alloc_objs.insert(obj);
  totalMonitored++;
  const std::unordered_set<gc_obj*>& linked = obj->getManagedChildren();
  for (auto iter=linked.begin(),end=linked.end(); iter!=end; iter++) {
    gc_obj* candidate = *iter;
    if (candidate && (int)alloc_objs.count(candidate)==0 && (int)root_objs.count(candidate)==0) {
      addAlloc(candidate);
    }
  }
}

//Assert that the object is allocated on the heap
void Collector::assertOnHeap(const gc_obj* obj) const {
  int stack_bottom = 0;
  assert(((reinterpret_cast<unsigned long long>(obj) < reinterpret_cast<unsigned long long>(_gc_stack_base)) ==
	(reinterpret_cast<unsigned long long>(obj) < reinterpret_cast<unsigned long long>(&stack_bottom)))
	&& "Object does not appear to be allocated on the heap");
}

