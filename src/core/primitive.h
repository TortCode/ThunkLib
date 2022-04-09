#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "thunk.h"


#define wrap(type, val) TWRAP(type)(val)
#define expose(type, val) TEXPOSE(type)(val)

//Boxes and unboxes primitive types into Values
    //Shorthands
#define VWRAP(type) Value_Wrap ## type
#define VEXPOSE(type) Value_Expose ## type
    //Definitions
#define WRAPINTO_VAL(base, type) \
static inline Value *VWRAP(type) (type t) { return VWRAP(base)(t); }
#define EXPOSEFROM_VAL(base, type) \
static inline type VEXPOSE(type) (Value *v) { return VEXPOSE(base)(v); }
#define BOX_VALUE(base, type) WRAPINTO_VAL(base, type) EXPOSEFROM_VAL(base, type)

//Box and unboxes Values into Thunks
    //Shorthands
#define TWRAP(type) Thunk_Wrap ## type
#define TEXPOSE(type) Thunk_Expose ## type
    //Declarations
#define WRAP_DECL(type) inline Thunk* TWRAP(type)(type);
#define EXPOSE_DECL(type) inline type TEXPOSE(type)(Thunk*);
#define BOX_DECL(type) WRAP_DECL(type) EXPOSE_DECL(type)

BOX_DECL(Int8)  BOX_DECL(Int16) BOX_DECL(Int32) BOX_DECL(Int64)
BOX_DECL(UInt8) BOX_DECL(UInt16) BOX_DECL(UInt32) BOX_DECL(UInt64)
BOX_DECL(Float32) BOX_DECL(Float64)
BOX_DECL(Bool)
BOX_DECL(Char)
    //Definitions
#define WRAPPER(type) \
extern inline Thunk* TWRAP(type)(type r) { \
    Value *v = VWRAP(type)(r);              \
    Thunk *t = Thunk_WrapValue(v);         \
    free(v);\
    return t; \
}
#define EXPOSER(type) \
extern inline type TEXPOSE(type)(Thunk *v) { \
    wparam(v, \
        type t = VEXPOSE(type)(Thunk_ExposeValue(v)); \
    ) \
    return t; \
}
#define BOX_THUNK(type) WRAPPER(type) EXPOSER(type)


//Declarations of builtin functions
GETFUNC_DECL(negI) GETFUNC_DECL(negL) GETFUNC_DECL(negF) GETFUNC_DECL(negD)
GETFUNC_DECL(not)

GETFUNC_DECL(addI) GETFUNC_DECL(addL) GETFUNC_DECL(addF) GETFUNC_DECL(addD)
GETFUNC_DECL(subI) GETFUNC_DECL(subL) GETFUNC_DECL(subF) GETFUNC_DECL(subD)
GETFUNC_DECL(mulI) GETFUNC_DECL(mulL) GETFUNC_DECL(mulF) GETFUNC_DECL(mulD)
GETFUNC_DECL(divI) GETFUNC_DECL(divL) GETFUNC_DECL(divF) GETFUNC_DECL(divD)
GETFUNC_DECL(modI) GETFUNC_DECL(modL)

GETFUNC_DECL(and)
GETFUNC_DECL(or)
GETFUNC_DECL(eq)
GETFUNC_DECL(neq)
GETFUNC_DECL(lt)
GETFUNC_DECL(lte)
GETFUNC_DECL(gt)
GETFUNC_DECL(gte)

GETFUNC_DECL(compose)
GETFUNC_DECL(app)

#endif