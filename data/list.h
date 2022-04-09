#ifndef LIST_H
#define LIST_H

#include "core/thunk.h"
#include "core/utils.h"

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