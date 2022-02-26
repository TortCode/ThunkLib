#include "thunk.h"

#define CONSTRUCTOR(val) \
Thunk *Make ## val ## (Size argc, ...) { \
    wvarargs(list, argc, \
        Thunk *t = Thunk_ValueWrap(Value_ConstructVAList(val, argc, list)); \
    ) \
    return t; \
}
#define ACCESSOR(name, idx) \
extern inline Thunk *Get ## name(Value *v) { \
    return v->fields[idx]; \
}
enum List{
    Nil,
    Cons
};