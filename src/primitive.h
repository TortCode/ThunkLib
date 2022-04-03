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

#define EXEC_SIG(name, args) Thunk *name(Thunk **args)

#define TWRAP(type) Thunk_Wrap ## type
#define TEXPOSE(type) Thunk_Expose ## type
#define VWRAP(type) Value_Wrap ## type
#define VEXPOSE(type) Value_Expose ## type

#define WRAPVAL_INTO(base, type) \
static inline Value *VWRAP(type) (type t) { return VWRAP(base)(t); }
#define EXPOSEVAL_FROM(base, type) \
static inline type VEXPOSE(type) (Value *v) { return VEXPOSE(base)(v); }
#define BOX_VALUE(base, type) WRAPVAL_INTO(base, type) EXPOSEVAL_FROM(base, type)

#define WRAP_DECL(type) inline Thunk* TWRAP(type)(type);
#define EXPOSE_DECL(type) inline type TEXPOSE(type)(Thunk*);
#define BOX_DECL(type) WRAP_DECL(type) EXPOSE_DECL(type)

BOX_DECL(Int8) BOX_DECL(Int16) BOX_DECL(Int32) BOX_DECL(Int64)
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

GETTER_DECL(Neg) GETTER_DECL(NegL) GETTER_DECL(NegF) GETTER_DECL(NegD)
GETTER_DECL(Not)

GETTER_DECL(Add) GETTER_DECL(AddL) GETTER_DECL(AddF) GETTER_DECL(AddD)
GETTER_DECL(Sub) GETTER_DECL(SubL) GETTER_DECL(SubF) GETTER_DECL(SubD)
GETTER_DECL(Mul) GETTER_DECL(MulL) GETTER_DECL(MulF) GETTER_DECL(MulD)
GETTER_DECL(Div) GETTER_DECL(DivL) GETTER_DECL(DivF) GETTER_DECL(DivD)
GETTER_DECL(Mod) GETTER_DECL(ModL)

GETTER_DECL(And)
GETTER_DECL(Or)
GETTER_DECL(Eq)
GETTER_DECL(Neq)
GETTER_DECL(Lt)
GETTER_DECL(Lte)
GETTER_DECL(Gt)
GETTER_DECL(Gte)

GETTER_DECL(Comp)
GETTER_DECL(App)

#endif