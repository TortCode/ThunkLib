#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "thunk.h"
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

#define FUNC_GETTER(name, func, arity) \
Thunk* name() { \
    static Thunk* f = NULL; \
    if (!f) f = Thunk_FuncWrap(func, arity); \
    return f; \
}

#define WRAPVAL_INTO(base, type) \
inline Value *Value_ ## type ## Wrap(type t) { return Value_ ## base ## Wrap(t); }
#define EXPOSEVAL_FROM(base, type) \
inline type Value_ ## type ## Expose(Value *v) { return Value_ ## base ## Expose(v); }
#define BOX_VALUE(base, type) WRAPVAL_INTO(base, type) EXPOSEVAL_FROM(base, type)

#define WRAP_DECL(type) inline Thunk* Thunk_ ## type ## Wrap(type);
#define EXPOSE_DECL(type) inline type Thunk_ ## type ## Expose(Thunk*);
#define BOX_DECL(type) WRAP_DECL(type) EXPOSE_DECL(type)

BOX_DECL(Int8)
BOX_DECL(Int16)
BOX_DECL(Int32)
BOX_DECL(Int64)
BOX_DECL(UInt8)
BOX_DECL(UInt16)
BOX_DECL(UInt32)
BOX_DECL(UInt64)
BOX_DECL(Float)
BOX_DECL(Double)
BOX_DECL(Bool)
BOX_DECL(Char)

#define WRAPPER(type) \
extern inline Thunk* Thunk_ ## type ## Wrap(type v) { \
    return Thunk_ValueWrap( Value_ ## type ## Wrap(v)); \
}
#define EXPOSER(type) \
extern inline type Thunk_ ## type ## Expose(Thunk *v) { \
    wparam(v, \
        type t = Value_ ## type ## Expose(Thunk_ValueExpose(v)); \
    ) \
    return t; \
}

#define TWRAP(type) Thunk_ ## type ## Wrap
#define TEXPOSE(type) Thunk_ ## type ## Expose

#define GETTER_DECL(name) Thunk* name();

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

#endif