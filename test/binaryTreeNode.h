
#include <unordered_set>
#include "../src/include/gc_obj.h"
#include "../src/include/collector.h"

class BinaryTreeNode : public gc_obj {
public:
	BinaryTreeNode(const int id);
	int size() const;
	void curtailToLevel(const int lvl);
	void extendToLevel(const int size,Collector* collector);
	void addLeftChild(BinaryTreeNode* leftChild);
	void addRightChild(BinaryTreeNode* rightChild);
	virtual void finalize();
	virtual std::unordered_set<gc_obj*> getManagedChildren();
private:
	int id;
	BinaryTreeNode* leftChild;
	BinaryTreeNode* rightChild;
};


