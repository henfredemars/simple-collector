
#include "include/gc_obj.h"

gc_obj::gc_obj() {
  //Do nothing
}

//Recursively mark objects
void gc_obj::_setMarkStateRecursive(MarkState ms) {
  if (_getMarkState()==ms) return;
  _setLocalMarkState(ms);
  const std::unordered_set<gc_obj*>& children = getManagedChildren();
  for (auto iter=children.begin(),end=children.end(); iter!=end; iter++) {
    (*iter)->_setMarkStateRecursive(ms);
  }
}

//Mark just this object
void gc_obj::_setLocalMarkState(MarkState ms) {
  _gc_tag = ms;
}

MarkState gc_obj::_getMarkState() const {
  return _gc_tag;
}

//Object finalizer
void gc_obj::finalize() {
  //Do nothing by default
}

