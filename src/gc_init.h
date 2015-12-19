
//Record the base of the stack to prevent accidental collection of stack variables

unsigned long long _gc_stack_base;

#define GC_INIT() do { int _gc_stack_base_marker = 0; _gc_stack_base = reinterpret_cast<unsigned long long>(&_gc_stack_base_marker); } while (false)

