#include "Primitive.h"

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
VWRAP_SIG(Int64, l) {
	DECLPTR(Value, data);
	data->_tag = l;
	data->_fieldc = 0;
	data->_fields = NULL;
	return data;
}
VEXPOSE_SIG(Int64, v) { return v->_tag; }
BOX_VALUE(Int32, UInt8)
BOX_VALUE(Int32, UInt16)
BOX_VALUE(Int32, UInt32)
BOX_VALUE(Int64, UInt64)
VWRAP_SIG(Float32, f) { return Value_WrapInt32(*((Int32 *)&f)); }
VEXPOSE_SIG(Float32, v) { Int32 i = Value_ExposeInt32(v); return *((Float32 *) &i); }
VWRAP_SIG(Float64, d) { return Value_WrapInt64(*((Int64 *)&d)); }
VEXPOSE_SIG(Float64, v) { Int64 l = Value_ExposeInt64(v); return *((Float64 *)&l); }
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
BOX_THUNK(Float32)
BOX_THUNK(Float64)
BOX_THUNK(Bool)
BOX_THUNK(Char)

//Generalized definition of a unary operator
#define UNOP(name, type, op) \
define_func(name, 1, args, return wrap(type, (op expose(type,args[0]))); )
//Generalized definition of a binary operator
#define BINOP(name, type, op) \
define_func(name, 2, args, return wrap(type, (expose(type, args[0]) op expose(type, args[1]))); )                              \

//Gives variations of operator for each type
#define OP_FORALLNUMS(name, op, kind) \
kind(name ## B, Int8, op)             \
kind(name ## S, Int16, op)            \
kind(name ## I, Int32, op)            \
kind(name ## L, Int64, op)            \
kind(name ## F, Float32, op)          \
kind(name ## D, Float64, op)

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

define_func(compose, 3, args, return Apply(args[0], Apply(args[1], args[2])); )
define_func(app, 2, args, return Apply(args[0], args[1]); )

