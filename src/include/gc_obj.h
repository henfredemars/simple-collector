
//Parent to all garbage-collected objects in the collected heap

#ifndef __GC_OBJ_H
#define __GC_OBJ_H

#include <cassert>
#include <unordered_set>

extern unsigned long long _gc_stack_base;

enum MarkState {
  GC_WHITE_MARK,
  GC_BLACK_MARK
};

class gc_obj {
public:
	gc_obj();
	~gc_obj() = delete; //No destructors allowed
	void _setMarkStateRecursive(MarkState ms);
	void _setLocalMarkState(MarkState ms);
	MarkState _getMarkState() const;
	virtual std::unordered_set<gc_obj*> getManagedChildren() = 0;
	virtual void finalize();

	//No moving or copying. The object must stay on the heap
        gc_obj& operator=(const gc_obj&) = delete;
        gc_obj(gc_obj&&) = delete;
        gc_obj(const gc_obj&) = delete;
        gc_obj& operator=(gc_obj&&) = delete;
private:
	MarkState _gc_tag;
};

#endif /* __GC_OBJ_H */


