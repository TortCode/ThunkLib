#include "primitive.h"

#define VWRAP_SIG(type, var) \
static Value *VWRAP(type)(type var)
#define VEXPOSE_SIG(type, var) \
static type VEXPOSE(type)(Value *var)

static Value *Value_WrapInt32(Int32);
static Int32 Value_ExposeInt32(Value*);

BOX_VALUE(Int32, Int8)
BOX_VALUE(Int32, Int16)
VWRAP_SIG(Int32, i) {
	DECLPTR(Value, data);
	data->_tag = i;
	data->_fieldc = 0;
	data->_fields = NULL;
	return data;
}
VEXPOSE_SIG(Int32, v) { return v->_tag; }
#if defined USE_INT64
VWRAP_SIG(Int64, l) {
	DECLPTR(Value, data);
	data->_tag = l;
	data->_fieldc = 0;
	data->_fields = NULL;
	return data;
}
VEXPOSE_SIG(Int64, v) { return v->_tag; }
#else
VWRAP_SIG(Int64, l) {
	DECLPTR(Value, data);
	data->_tag = (Int32)(l >> 32);
	data->_fieldc = (Int32)l;
	data->_fields = NULL;
	return data;
}
VEXPOSE_SIG(Int64, v) { return (Int64)v->_tag << 32 | (v->_fieldc & 0xFFFFFFFFLL); }
#endif
BOX_VALUE(Int32, UInt8)
BOX_VALUE(Int32, UInt16)
BOX_VALUE(Int32, UInt32)
BOX_VALUE(Int64, UInt64)
VWRAP_SIG(Float, f) { return Value_WrapInt32(*((Int32 *)&f)); }
VEXPOSE_SIG(Float, v) { Int32 i = Value_ExposeInt32(v); return *((Float *) &i); }
VWRAP_SIG(Double, d) { return Value_WrapInt64(*((Int64 *)&d)); }
VEXPOSE_SIG(Double, v) { Int64 l = Value_ExposeInt64(v); return *((Double *)&l); }
BOX_VALUE(Int32, Bool)
BOX_VALUE(Int32, Char)


BOX_THUNK(Int8)
BOX_THUNK(Int16)
BOX_THUNK(Int32)
BOX_THUNK(Int64)
BOX_THUNK(UInt8)
BOX_THUNK(UInt16)
BOX_THUNK(UInt32)
BOX_THUNK(UInt64)
BOX_THUNK(Float)
BOX_THUNK(Double)
BOX_THUNK(Bool)
BOX_THUNK(Char)

#define UNOP(name, type, op) \
FUNC_DEF(name, 1, args, return wrap(type, (op expose(type,args[0]))); )

#define BINOP(name, type, op) \
FUNC_DEF(name, 2, args, return wrap(type, (expose(type, args[0]) op expose(type, args[1]))); )                              \

#define OP_FORALLNUMS(name, op, kind) \
kind(name ## B, Int8, op)             \
kind(name ## S, Int16, op)            \
kind(name ## I, Int32, op)            \
kind(name ## L, Int64, op)            \
kind(name ## F, Float, op)            \
kind(name ## D, Double, op)

OP_FORALLNUMS(neg, -, UNOP)
UNOP(not, Bool, !)

OP_FORALLNUMS(add, +, BINOP)
OP_FORALLNUMS(sub, -, BINOP)
OP_FORALLNUMS(mul, *, BINOP)
OP_FORALLNUMS(div, /, BINOP)
BINOP(modI, Int32, %) BINOP(modL, Int64, %)

BINOP(and, Bool, &&)
BINOP(or, Bool, ||)
BINOP(eq, Bool, ==)
BINOP(neq, Bool, !=)
BINOP(lt, Bool, <)
BINOP(lte, Bool, <=)
BINOP(gt, Bool, >)
BINOP(gte, Bool, >=)

FUNC_DEF(compose, 3, args, return Apply(args[0], Apply(args[1], args[2])); )

FUNC_DEF(app, 2, args, return Apply(args[0], args[1]); )

