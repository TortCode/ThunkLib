#ifndef THUNKLIB__CORE__PRIMITIVE_H
#define THUNKLIB__CORE__PRIMITIVE_H
#include "Thunk.h"

namespace Core::Primitive
{
#define wrap(type, val) PRIMITIVE_NAMESPACE::PWRAP(type)(val)
#define expose(type, val) PRIMITIVE_NAMESPACE::PEXPOSE(type)(val)

    //Boxes and unboxes primitive types into Values
        //Shorthands
#define VWRAP(type) Value_Wrap ## type
#define VEXPOSE(type) Value_Expose ## type
        //Definitions
#define WRAPINTO_VAL(base, type) \
    static inline THUNK_NAMESPACE::Value *VWRAP(type) (type t) { return VWRAP(base)(t); }
#define EXPOSEFROM_VAL(base, type) \
    static inline type VEXPOSE(type) (THUNK_NAMESPACE::Value *v) { return VEXPOSE(base)(v); }
#define BOX_VALUE(base, type) WRAPINTO_VAL(base, type) EXPOSEFROM_VAL(base, type)


    //Box and unboxes Values into Thunks
        //Shorthands
#define PWRAP(type) Wrap ## type
#define PEXPOSE(type) Expose ## type
        //Declarations
#define WRAP_DECL(type) inline THUNK_NAMESPACE::Thunk* PWRAP(type)(type);
#define EXPOSE_DECL(type) inline type PEXPOSE(type)(THUNK_NAMESPACE::Thunk*);
#define BOX_DECL(type) WRAP_DECL(type) EXPOSE_DECL(type)

    BOX_DECL(Int8)  BOX_DECL(Int16) BOX_DECL(Int32) BOX_DECL(Int64)
    BOX_DECL(UInt8) BOX_DECL(UInt16) BOX_DECL(UInt32) BOX_DECL(UInt64)
    BOX_DECL(Float32) BOX_DECL(Float64)
    BOX_DECL(Bool)
    BOX_DECL(Char)
        //Definitions
#define WRAPPER(type) \
    extern inline THUNK_NAMESPACE::Thunk* PWRAP(type)(type r) { \
        using namespace THUNK_NAMESPACE;        \
        Value *v = VWRAP(type)(r);              \
        Thunk *t = WrapValue(v);         \
        free(v);\
        return t; \
    }
#define EXPOSER(type) \
    extern inline type PEXPOSE(type)(Thunk *v) { \
        using namespace THUNK_NAMESPACE;              \
        wparam(v, \
            type t = VEXPOSE(type)(ExposeValue(v)); \
        ) \
        return t; \
    }
#define BOX_THUNK(type) WRAPPER(type) EXPOSER(type)


#define DECL_FORALLNUMS(name) \
    GETFUNC_DECL(name ## S);      \
    GETFUNC_DECL(name ## I);      \
    GETFUNC_DECL(name ## L);      \
    GETFUNC_DECL(name ## F);      \
    GETFUNC_DECL(name ## D);      \

    //Declarations of builtin functions
    DECL_FORALLNUMS(ainv)
    GETFUNC_DECL(neg);

    DECL_FORALLNUMS(add)
    DECL_FORALLNUMS(sub)
    DECL_FORALLNUMS(mul)
    DECL_FORALLNUMS(div)
    GETFUNC_DECL(modI); GETFUNC_DECL(modL);

    GETFUNC_DECL(conj);
    GETFUNC_DECL(disj);
    GETFUNC_DECL(eq);
    GETFUNC_DECL(neq);
    GETFUNC_DECL(lt);
    GETFUNC_DECL(lte);
    GETFUNC_DECL(gt);
    GETFUNC_DECL(gte);

    GETFUNC_DECL(compose);
    GETFUNC_DECL(app);
};

#endif