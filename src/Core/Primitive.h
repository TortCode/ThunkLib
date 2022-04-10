#ifndef THUNKLIB__CORE__PRIMITIVE_H
#define THUNKLIB__CORE__PRIMITIVE_H
#include "Thunk.h"

#define namespace PRIMNS

#define wrap(type, val) TWRAP(type)(val)
#define expose(type, val) TEXPOSE(type)(val)

//Boxes and unboxes primitive types into Values
    //Shorthands
#define VWRAP(type) Value_Wrap ## type
#define VEXPOSE(type) Value_Expose ## type
    //Definitions
#define WRAPINTO_VAL(base, type) \
static qValue *VWRAP(type) (type t) { return VWRAP(base)(t); }
#define EXPOSEFROM_VAL(base, type) \
static type VEXPOSE(type) (qValue *v) { return VEXPOSE(base)(v); }
#define BOX_VALUE(base, type) WRAPINTO_VAL(base, type) EXPOSEFROM_VAL(base, type)


//Box and unboxes Values into Thunks
    //Shorthands
#define TWRAP(type) qp(Wrap ## type)
#define TEXPOSE(type) qp(Expose ## type)
    //Declarations
#define WRAP_DECL(type) qThunk* TWRAP(type)(type);
#define EXPOSE_DECL(type) type TEXPOSE(type)(qThunk*);
#define BOX_DECL(type) WRAP_DECL(type) EXPOSE_DECL(type)

BOX_DECL(Int8)  BOX_DECL(Int16) BOX_DECL(Int32) BOX_DECL(Int64)
BOX_DECL(UInt8) BOX_DECL(UInt16) BOX_DECL(UInt32) BOX_DECL(UInt64)
BOX_DECL(Float32) BOX_DECL(Float64)
BOX_DECL(Bool)
BOX_DECL(Char)
    //Definitions
#define WRAPPER(type) \
qThunk* TWRAP(type)(type r) { \
    qValue *v = VWRAP(type)(r);              \
    qThunk *t = qt(WrapValue)(v);         \
    free(v);\
    return t; \
}
#define EXPOSER(type) \
type TEXPOSE(type)(qThunk *v) { \
    wparam(v, \
        type t = VEXPOSE(type)(qt(ExposeValue)(v)); \
    ) \
    return t; \
}
#define BOX_THUNK(type) WRAPPER(type) EXPOSER(type)

#undef namespace


#endif