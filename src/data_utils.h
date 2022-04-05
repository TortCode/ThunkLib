#include "thunk.h"

#define GETFUNC(name, func, arity) \
Thunk* name() { \
    static Thunk* f = NULL; \
    if (!f) f = Thunk_WrapFunc(func, arity); \
    return f; \
}
#define GETFUNC_DECL(name) Thunk* name();

#define TMK(val) Make ## val

#define CONSTRUCTOR_DECL(val) Thunk *TMK(val)(Size, Thunk**);

#ifdef DEBUG
#define CONSTRUCTOR(val, len) \
Thunk *TMK(val)(Size argc, Thunk** args) { \
    if (argc != len) fprintf(stderr, "%d Arguments are Required", len);     \
    Thunk *t = Thunk_WrapValue(Value_Construct(val, argc, args)); \
    return t; \
}
#else
#define CONSTRUCTOR(val, len)                   \
Thunk *TMK(val)(Size argc, Thunk**args) {\
    return Thunk_WrapValue(Value_Construct(val, argc, args));\
}
#endif

#define mk(val, argc, ...) TMK(val)(argc, ThunkList_FromVA(argc __VA_OPT__(,) __VA_ARGS__))

#define ACCESSOR_DECL(name) inline Thunk *Get ## name(Value*);
#define ACCESSOR(name, idx) \
extern inline Thunk *Get ## name(Value *v) { \
    return v->fields[idx]; \
}

#define mkstruct(tag, fieldc, ...) Thunk_WrapValue(Value_Construct(tag, fieldc, __VA_ARGS__))

#ifdef DEBUG
#define get(thunk, idx) (!((thunk)->_val)? fprinf(stderr, "%s not fully evaluated yet!", # thunk) : (thunk)->_val->_fields[idx])
#else
#define get(thunk, idx) ((thunk)->_val->_fields[idx])
#endif

