
#include "../src/include/collector.h"
#include "../src/gc_init.h"
#include "binaryTreeNode.h"

int main(int arc, char** argv) {
	//Record stack base
	GC_INIT();

	//Instantiate collector
	Collector collector;

	//Preliminary collector statistics
	printf("Preliminary Collector Statistics:\n\n");
	collector.printStats();

	//Build binary tree
	BinaryTreeNode* node = new BinaryTreeNode(999);
	node->extendToLevel(4);
	collector.addRoot(node);

	//Compute size of the tree
	printf("Tree size: %d nodes\n", node->size());

	//More statistics
	printf("Statistics after tree is built:\n\n");
	collector.printStats();

	//Curtail tree
	printf("Tree has been curtailed\n");
	node->curtailToLevel(3);
	printf("Tree size: %d nodes\n", node->size());

	//Explicit call to the collector
	collector.collect();
	printf("Tree size: %d nodes\n", node->size());
}
