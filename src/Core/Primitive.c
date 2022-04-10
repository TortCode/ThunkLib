#include "Primitive.h"

#define namespace PRIMNS

#define VWRAP_SIG(type, var) \
static qValue *VWRAP(type)(type var)
#define VEXPOSE_SIG(type, var) \
static type VEXPOSE(type)(qValue *var)

static qValue *Value_WrapInt32(Int32);
static Int32 Value_ExposeInt32(qValue*);

BOX_VALUE(Int32, Int8)
BOX_VALUE(Int32, Int16)
VWRAP_SIG(Int32, i) {
    DECLPTR(qValue, data);
    data->_tag = i;
    data->_fieldc = 0;
    data->_fields = NULL;
    return data;
}
VEXPOSE_SIG(Int32, v) { return v->_tag; }
VWRAP_SIG(Int64, l) {
    DECLPTR(qValue, data);
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

#undef namespace


