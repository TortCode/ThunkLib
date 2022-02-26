#include "primitive.h"

Value *Value_Int32Wrap(Int32);
Int32 *Value_int32Expose(Thunk*);

BOX_VALUE(Int32, Int8)
BOX_VALUE(Int32, Int16)
Value *Value_Int32Wrap(Int32 i) {
	declptr(Value, data);
	data->_tag = i;
	data->_fieldc = 0;
	data->_fields = NULL;
	return data;
}
Int32 Value_Int32Expose(Value *v) { return v->_tag; }
#if defined USE_INT64
Value *Value_Int64Wrap(Int64 l) {
	declptr(Value, data);
	data->_tag = l;
	data->_fieldc = 0;
	data->_fields = NULL;
	return data;
}
Int64 Value_Int64Expose(Value *v) { return v->_tag; }
#else
Value *Value_Int64Wrap(Int64 l) {
	declptr(Value, data);
	data->_tag = (Int32)(l >> 32);
	data->_fieldc = (Int32)l;
	data->_fields = NULL;
	return data;
}
Int64 Value_Int64Expose(Value *v) { return (Int64)v->_tag << 32 | (v->_fieldc & 0xFFFFFFFFLL); }
#endif
BOX_VALUE(Int32, UInt8)
BOX_VALUE(Int32, UInt16)
BOX_VALUE(Int32, UInt32)
BOX_VALUE(Int64, UInt64)
Value *Value_FloatWrap(Float f) { return Value_Int32Wrap(*((Int32 *)&f)); }
Float Value_FloatExpose(Value *v) { Int32 i = Value_Int32Expose(v); return *((Float *) &i); }
Value *Value_DoubleWrap(Double d) { return Value_Int64Wrap(*((Int64 *)&d)); }
Double Value_DoubleExpose(Value *v) { Int64 l = Value_Int64Expose(v); return *((Double *)&l); }
BOX_VALUE(Int32, Bool)
BOX_VALUE(Int32, Char)

#define BOX_THUNK(type) WRAPPER(type) EXPOSER(type)
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

#define UNOP_DEF(name, type, op) \
Thunk* name(Thunk** args) { \
	return TWRAP(type)( op TEXPOSE(type)(args[0]) ); \
}
#define UNOP(name, type, op) \
UNOP_DEF(name ## _func, type, op) \
FUNC_GETTER(name, name ## _func, 1)

#define BINOP_DEF(name, type, op) \
Thunk* name(Thunk** args) { \
	return TWRAP(type)( TEXPOSE(type)(args[0]) op TEXPOSE(type)(args[1]) ); \
}
#define BINOP(name, type, op) \
BINOP_DEF(name ## _func, type, op) \
FUNC_GETTER(name, name ## _func, 2)

UNOP(Neg, Int32, -) UNOP(NegL, Int64, -) UNOP(NegF, Float, -) UNOP(NegD, Double, -)
UNOP(Not, Bool, !)

BINOP(Add, Int32, +) BINOP(AddL, Int64, +) UNOP(AddF, Float, -) UNOP(AddD, Double, -)
BINOP(Sub, Int32, -) BINOP(SubL, Int64, -) UNOP(SubF, Float, -) UNOP(SubD, Double, -)
BINOP(Mul, Int32, *) BINOP(MulL, Int64, *) UNOP(MulF, Float, -) UNOP(MulD, Double, -)
BINOP(Div, Int32, %) BINOP(DivL, Int64, %) UNOP(DivF, Float, -) UNOP(DivD, Double, -)
BINOP(Mod, Int32, %) BINOP(ModL, Int64, %)

BINOP(And, Bool, &&)
BINOP(Or, Bool, ||)
BINOP(Eq, Bool, ==)
BINOP(Neq, Bool, !=)
BINOP(Lt, Bool, <)
BINOP(Lte, Bool, <=)
BINOP(Gt, Bool, >)
BINOP(Gte, Bool, >=)

