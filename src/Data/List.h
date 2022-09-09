#ifndef THUNKLIB__DATA__LIST_H
#define THUNKLIB__DATA__LIST_H

#include "idx.h"

#define namespace qu(Data, List)

data(List)
{
    qtag(Nil),
    qtag(Cons)
};

decl_constructor(Nil);

decl_constructor(Cons);

decl_func(map);

decl_func(filter);

decl_func(foldl);

decl_func(foldr);

#undef namespace

#endif