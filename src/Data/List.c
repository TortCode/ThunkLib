#include "List.h"


#define namespace qu(Data, List)

constructor(List, Nil, 0)

constructor(List, Cons, 2)


define_func(map, 2, args,
    qThunk
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
                ap(f, get(l, 0)),
                mulap(q(map)(), f, get(l, 1)));
        }
    }
)

define_func(filter, 2, args,
    qThunk
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
                qThunk * x = get(l, 0), *xs = get(l, 1);
                return qmk(Cons, x, mulap(q(filter)(), p, xs));
            }
            else
                return mulap(q(filter)(), p, get(l, 1));
        }
    }
)

define_func(foldl, 3, args,
    qThunk
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
            qThunk * x = get(l, 0), *xs = get(l, 1);
            return mulap(q(foldl)(), f, mulap(f, acc, x), xs);
        }
    }
)

define_func(foldr, 3, args,
    qThunk
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
            qThunk * x = get(l, 0), *xs = get(l, 1);
            return mulap(f, x, mulap(q(foldr)(), f, acc, xs));
        }
    }
)

#undef namespace

