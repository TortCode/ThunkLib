#ifndef THUNKLIB_OPERATOR_H
#define THUNKLIB_OPERATOR_H

#include "Utils.h"
#include "Primitive.h"

#define namespace OPERNS

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

#undef namespace

#endif
