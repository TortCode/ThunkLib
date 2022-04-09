#ifndef THUNKLIB__DATA__LIST_H
#define THUNKLIB__DATA__LIST_H

#include "Core.h"

enum List{
    CON2TAG(Nil),
    CON2TAG(Cons)
};

CONSTRUCTOR_DECL(Nil)
CONSTRUCTOR_DECL(Cons)

GETFUNC_DECL(map)

GETFUNC_DECL(filter)

GETFUNC_DECL(foldl)

GETFUNC_DECL(foldr)

#endif