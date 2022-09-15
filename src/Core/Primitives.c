#include "Primitives.h"

#define namespace PRIMNS

#define VWRAP_SIG(type, var) \
static Value *VWRAP(type)(type var)

#define VEXPOSE_SIG(type, var) \
static type VEXPOSE(type)(Value *var)

static Value *Value_WrapInt32(Int32);
static Int32 Value_ExposeInt32(Value*);
static Value *Value_WrapInt64(Int64);
static Int64 Value_ExposeInt64(Value*);

BOX_VALUE(Int32, Int8)
BOX_VALUE(Int32, Int16)
VWRAP_SIG(Int32, l) { return qt(Construct)(l, 0, NULL); }
VEXPOSE_SIG(Int32, v) { return qt(Tag)(v); }
VWRAP_SIG(Int64, l)
{
    Int32 hi = (Int32) (l >> 32 & 0xFF);
    Int32 lo = (Int32) (l & 0xFF);
    return qt(Construct)(hi, lo, NULL);
}
VEXPOSE_SIG(Int64, v)
{
    Int32 hi = qt(Tag)(v);
    Int32 lo = qt(Fieldc)(v);
    return (hi << 32) | lo;
}
BOX_VALUE(Int32, UInt8)
BOX_VALUE(Int32, UInt16)
BOX_VALUE(Int32, UInt32)
BOX_VALUE(Int64, UInt64)
VWRAP_SIG(Float32, f) { return Value_WrapInt32(cast_to(Int32, f)); }
VEXPOSE_SIG(Float32, v) { Int32 i = Value_ExposeInt32(v); return cast_to(Float32, i); }
VWRAP_SIG(Float64, d) { return Value_WrapInt64(cast_to(Int64, d)); }
VEXPOSE_SIG(Float64, v) { Int64 l = Value_ExposeInt64(v); return cast_to(Float64, l); }
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


