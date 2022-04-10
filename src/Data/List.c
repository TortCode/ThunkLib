#include "List.h"


namespace Data::List
{

    CONSTRUCTOR(List, Nil, 0)

    CONSTRUCTOR(List, Cons, 2)

    define_func(map, 2, args,
        using namespace THUNK_NAMESPACE;
        Thunk * f = args[0], *l = args[1];
        caseof(l) {
            pat(List, Nil):
            return mk(Nil);
            pat(List, Cons):
            return mk(Cons,
                ap(f, get(l, 0)),
                mulap(map(), f, get(l, 1)));
        }
    )

    define_func(filter, 2, args,
        using namespace THUNK_NAMESPACE;
        Thunk *p = args[0];
        Thunk *l = args[1];
        caseof(l)
        {
            pat(List, Nil):
            {
                return mk(Nil);
            }
            pat(List, Cons):
            {
                if (expose(Bool, ap(p, get(l, 0))))
                {
                    Thunk * x = get(l, 0), *xs = get(l, 1);
                    return mk(Cons, x, mulap(filter(), p, xs));
                } else
                    return mulap(filter(), p, get(l, 1));
            }
        }
    )

    define_func(foldl, 3, args,
        using namespace THUNK_NAMESPACE;
        Thunk * f = args[0], *acc = args[1], *l = args[2];
        caseof(l)
        {
            pat(List, Nil):
            {
                return acc;
            }
            pat(List, Cons):
            {
                Thunk * x = get(l, 0), *xs = get(l, 1);
                return mulap(foldl(), f, mulap(f, acc, x), xs);
            }
        }
    )

    define_func(foldr, 3, args,
        using namespace THUNK_NAMESPACE;
        Thunk * f = args[0], *acc = args[1], *l = args[2];
        caseof(l)
        {
            pat(List, Nil):
            {
                return acc;
            }
            pat(List, Cons):
            {
                Thunk * x = get(l, 0), *xs = get(l, 1);
                return mulap(f, x, mulap(foldr(), f, acc, xs));
            }
        }
    )
}

