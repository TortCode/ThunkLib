#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "thunk.h"
#include "utils.h"
#include <stdbool.h>

typedef int8_t Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;
typedef uint8_t UInt8;
typedef uint16_t UInt16;
typedef uint32_t UInt32;
typedef uint64_t UInt64;
typedef float Float;
typedef double Double;
typedef bool Bool;
typedef char Char;

#define TWRAP(type) Thunk_Wrap ## type
#define TEXPOSE(type) Thunk_Expose ## type
#define VWRAP(type) Value_Wrap ## type
#define VEXPOSE(type) Value_Expose ## type

#define wrap(type, val) TWRAP(type)(val)
#define expose(type, val) TEXPOSE(type)(val)

#define WRAPINTO_VAL(base, type) \
static inline Value *VWRAP(type) (type t) { return VWRAP(base)(t); }
#define EXPOSEFROM_VAL(base, type) \
static inline type VEXPOSE(type) (Value *v) { return VEXPOSE(base)(v); }
#define BOX_VALUE(base, type) WRAPINTO_VAL(base, type) EXPOSEFROM_VAL(base, type)

#define WRAP_DECL(type) inline Thunk* TWRAP(type)(type);
#define EXPOSE_DECL(type) inline type TEXPOSE(type)(Thunk*);
#define BOX_DECL(type) WRAP_DECL(type) EXPOSE_DECL(type)

BOX_DECL(Int8)  BOX_DECL(Int16) BOX_DECL(Int32) BOX_DECL(Int64)
BOX_DECL(UInt8) BOX_DECL(UInt16) BOX_DECL(UInt32) BOX_DECL(UInt64)
BOX_DECL(Float) BOX_DECL(Double)
BOX_DECL(Bool)
BOX_DECL(Char)

#define WRAPPER(type) \
extern inline Thunk* TWRAP(type)(type v) { \
    return Thunk_WrapValue( Value_Wrap ## type(v)); \
}
#define EXPOSER(type) \
extern inline type TEXPOSE(type)(Thunk *v) { \
    wparam(v, \
        type t = Value_Expose ## type(Thunk_ExposeValue(v)); \
    ) \
    return t; \
}
#define BOX_THUNK(type) WRAPPER(type) EXPOSER(type)

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

GETFUNC_DECL(comp)
GETFUNC_DECL(app)

#endif