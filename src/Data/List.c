#include "List.h"


#define namespace qu(Data, List)

constructor(List, Nil, 0)

constructor(List, Cons, 2)


define_func(map, 2, args,
    Thunk
    *f = args[0],
    *l = args[1];
    caseof(l)
    {
        qpat(Nil):
        {
            return qmk(Nil);
        }
        qpat(Cons):
        {
            return qmk(Cons,
                ap(f, get(l, 1)),
                mlp(q(map)(), f, get(l, 0)));
        }
    }
)

define_func(filter, 2, args,
    Thunk
    *p = args[0],
    *l = args[1];
    caseof(l)
    {
        qpat(Nil):
        {
            return qmk(Nil);
        }
        qpat(Cons):
        {
            if (expose(Bool, ap(p, get(l, 0))))
            {
                Thunk * x = get(l, 0), *xs = get(l, 1);
                return qmk(Cons, x, mlp(q(filter)(), p, xs));
            }
            else
                return mlp(q(filter)(), p, get(l, 1));
        }
    }
)

define_func(foldl, 3, args,
    Thunk
    *f = args[0],
    *acc = args[1],
    *l = args[2];
    caseof(l)
    {
        qpat(Nil):
        {
            return acc;
        }
        qpat(Cons):
        {
            Thunk * x = get(l, 0), *xs = get(l, 1);
            return mlp(q(foldl)(), f, mlp(f, acc, x), xs);
        }
    }
)

define_func(foldr, 3, args,
    Thunk
    *f = args[0],
    *acc = args[1],
    *l = args[2];
    caseof(l)
    {
        qpat(Nil):
        {
            return acc;
        }
        qpat(Cons):
        {
            Thunk * x = get(l, 0), *xs = get(l, 1);
            return mlp(f, x, mlp(q(foldr)(), f, acc, xs));
        }
    }
)

#undef namespace

