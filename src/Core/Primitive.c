#include "Primitive.h"

#define namespace PRIMNS

#define VWRAP_SIG(type, var) \
static Value *VWRAP(type)(type var)
#define VEXPOSE_SIG(type, var) \
static type VEXPOSE(type)(Value *var)

static Value *Value_WrapInt64(Int64);
static Int64 Value_ExposeInt64(Value*);

BOX_VALUE(Int64, Int8)
BOX_VALUE(Int64, Int16)
BOX_VALUE(Int64, Int32)
VWRAP_SIG(Int64, l) { return qt(Construct)(l, 0, NULL); }
VEXPOSE_SIG(Int64, v) { return qt(Tag)(v); }
BOX_VALUE(Int64, UInt8)
BOX_VALUE(Int64, UInt16)
BOX_VALUE(Int64, UInt32)
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

#undef namespace


