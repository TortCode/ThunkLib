#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "thunk.h"
#include "data_utils.h"
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

GETFUNC_DECL(Neg) GETFUNC_DECL(NegL) GETFUNC_DECL(NegF) GETFUNC_DECL(NegD)
GETFUNC_DECL(Not)

GETFUNC_DECL(Add) GETFUNC_DECL(AddL) GETFUNC_DECL(AddF) GETFUNC_DECL(AddD)
GETFUNC_DECL(Sub) GETFUNC_DECL(SubL) GETFUNC_DECL(SubF) GETFUNC_DECL(SubD)
GETFUNC_DECL(Mul) GETFUNC_DECL(MulL) GETFUNC_DECL(MulF) GETFUNC_DECL(MulD)
GETFUNC_DECL(Div) GETFUNC_DECL(DivL) GETFUNC_DECL(DivF) GETFUNC_DECL(DivD)
GETFUNC_DECL(Mod) GETFUNC_DECL(ModL)

GETFUNC_DECL(And)
GETFUNC_DECL(Or)
GETFUNC_DECL(Eq)
GETFUNC_DECL(Neq)
GETFUNC_DECL(Lt)
GETFUNC_DECL(Lte)
GETFUNC_DECL(Gt)
GETFUNC_DECL(Gte)

GETFUNC_DECL(Comp)
GETFUNC_DECL(App)

#endif