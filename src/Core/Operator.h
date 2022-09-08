#ifndef THUNKLIB_OPERATOR_H
#define THUNKLIB_OPERATOR_H

#include "Utils.h"
#include "Primitive.h"

#define namespace qu(Core, Operator)
#define OPERNS qu(Core, Operator)

#define DECL_FORALLNUMS(name) \
decl_func(name ## S);      \
decl_func(name ## I);      \
decl_func(name ## L);      \
decl_func(name ## F);      \
decl_func(name ## D);      \

//Declarations of builtin functions
DECL_FORALLNUMS(ainv)
decl_func(neg);

DECL_FORALLNUMS(add)
DECL_FORALLNUMS(sub)
DECL_FORALLNUMS(mul)
DECL_FORALLNUMS(div)
decl_func(modI); decl_func(modL);

decl_func(conj);
decl_func(disj);
decl_func(eq);
decl_func(neq);
decl_func(lt);
decl_func(lte);
decl_func(gt);
decl_func(gte);

decl_func(compose);
decl_func(app);

#undef namespace

#endif
