# simple-collector
A simple yet precise mark-sweep garbage collector for C++ object systems

simple-collector is a very small, single-threaded, precise, light-weight garbage collector for C++. It implements a simple mark-sweep algorithm on top of malloc()-free() without the bells and whistles. With simple-collector, you can:

0. Manage floating data structures such as graphs and trees with ease
0. Register finalizers that will always run to handle the eventual destruction of your object
0. Explicitly control when your application collects memory
0. Run multiple instances of the collector independently and in isolation
0. Add a tiny portable dependency with less than 500 lines of code to your application
0. Use the highly-permisive and worry-free MIT license

# Building the Collector

The collector is so small that usually you can link it directly into your application without too much concern for additional binary size. The collector consists of two small object files that are built according to the makefile. Simply download the source tree and run:

```
make clean
make all
```

The collector uses C++11 features (primarily for the auto keyword and deletion of implicitly-declared members) but otherwise shouldn't depend on external libraries. A test program is provided. It shouldn't be hard to port to older C++ if your application requires this. If nothing else, it provides a starting point.

# Usage

To make use of the collector, first call the macro GC_INIT() defined in src/gc_init.h as soon as possible, preferably as the first statement in main(). This performs any required initialization, which presently injects a small local variable to mark the boundary of your stack. This value is used to set stack boundaries and helps to reject attempts to delete objects that were incidentally or otherwise stack-allocated. 

Next, implement your garbage collected objects as subclasses of gc_obj declared in src/include/gc_obj.h. Override the getManagedChildren() pure virtual method to return any outgoing references to other collected objects. 

Decide on the root objects for your garbage collected area. In a tree structure, this would be the root node, or the main node of your object graph. A root object might be created expressly to reference other garbage-collected objects in your object pool. 

Finally, create an instance of Collector (src/include/collector.h) and add the root object with Collector.addRoot(obj). The collector will recursively scan the root and all of its connected children via the overriden getManagedChildren() methods. You may also add managed objects manually using Collector.addObject() on each object. When creating new objects, it is not enough that the new objects are returned as managed children later. You must explicitly call Collector.addObject(obj) when a new object should be subject to garbage collection. 

# Example

```
class BinaryTreeNode : public gc_obj {
public:
	BinaryTreeNode() {};
	void extendToLevel(const int size,Collector* collector);
	virtual void finalize() { printf("Finalize!\n"); }
	virtual std::unordered_set<gc_obj*> getManagedChildren();
private:
	BinaryTreeNode* leftChild;
	BinaryTreeNode* rightChild;
};

void BinaryTreeNode::extendToLevel(const int size,Collector* collector) {
	if (size==0) return;
	if (leftChild && rightChild) {
	  leftChild->extendToLevel(size-1,collector);
	  rightChild->extendToLevel(size-1,collector);
	} else if (!leftChild) {
	  leftChild = new BinaryTreeNode(size);
	  collector->addObject(leftChild);
	  this->extendToLevel(size,collector);
	} else {
	  rightChild = new BinaryTreeNode(size);
	  collector->addObject(rightChild);
	  this->extendToLevel(size,collector);
	}
}

std::unordered_set<gc_obj*> BinaryTreeNode::getManagedChildren() {
	std::unordered_set<gc_obj*> children;
	if (leftChild)
	  children.insert(leftChild);
	if (rightChild)
	  children.insert(rightChild);
	return children;
}

int main(int arc, char** argv) {
	GC_INIT();
	Collector collector;
	BinaryTreeNode* node = new BinaryTreeNode();
	collector.addRoot(node);
	node->extendToLevel(4,&collector);
	collector.collect();
}
```

# Limitations

0. The collector is not thread safe
0. The collector requires individual association with each new object to be collected
0. The collector is neither generational nor incremental (pause times may be noticable for large heaps)
