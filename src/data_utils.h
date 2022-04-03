#include "thunk.h"

#define FUNC_GETTER(name, func, arity) \
Thunk* name() { \
    static Thunk* f = NULL; \
    if (!f) f = Thunk_WrapFunc(func, arity); \
    return f; \
}
#define GETTER_DECL(name) Thunk* name();

#define CONSTRUCTOR_DECL(val) Thunk *Make ## val(Size);

#ifdef DEBUG
#define CONSTRUCTOR(val, len) \
Thunk *Make ## val(Size argc, ...) { \
    if (argc != len) fprintf(stderr, "%d Arguments are Required", len); \
    wvarargs(list, argc, \
        Thunk *t = Thunk_WrapValue(Value_ConstructVAList(val, argc, list)); \
    ) \
    return t; \
}
#else
#define CONSTRUCTOR(val, len) \
Thunk *Make ## val ## (Size argc, ...) { \
    wvarargs(list, argc, \
        Thunk *t = Thunk_WrapValue(Value_ConstructVAList(val, argc, list)); \
    ) \
    return t; \
}
#endif

#define ACCESSOR_DECL(name) inline Thunk *Get ## name(Value*);
#define ACCESSOR(name, idx) \
extern inline Thunk *Get ## name(Value *v) { \
    return v->fields[idx]; \
}

