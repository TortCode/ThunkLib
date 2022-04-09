#include "list.h"
#include "primitive.h"
#include <stdio.h>

CONSTRUCTOR(Nil, 0)
CONSTRUCTOR(Cons, 2)

static EXEC_SIG(map_func, args) {
    Thunk *f = args[0], *l = args[1];
    caseof(l) {
        pat(Nil):
            return mk(Nil);
        pat(Cons):
            return mk(Cons,
                      ap(f, get(l, 0)),
                      mulap(map(), f, get(l, 1)));
    }
}

GETFUNC(map, map_func, 2)

static EXEC_SIG(filter_func, args) {
    Thunk *p = args[0]; Thunk *l = args[1];
    caseof(l) {
        pat(Nil): {
            return mk(Nil);
        }
        pat(Cons): {
            if (expose(Bool, ap(p, get(l, 0)))) {
                Thunk *x = get(l, 0), *xs = get(l, 1);
                return mk(Cons, x, mulap(filter(), p, xs));
            }
            else
                return mulap(filter(), p, get(l, 1));
        }
    }
}

GETFUNC(filter, filter_func, 2)

static EXEC_SIG(foldl_func, args) {
    Thunk *f = args[0], *acc = args[1], *l = args[2];
#ifdef DEBUG
    printf("acc: %d \n", expose(Int32, acc));
#endif
    caseof(l) {
        pat(Nil): {
            return acc;
        }
        pat(Cons): {
            Thunk *x = get(l, 0), *xs = get(l, 1);
            return mulap(foldl(), f, mulap(f, acc, x), xs);
        }
    }
}

GETFUNC(foldl, foldl_func, 3)

static EXEC_SIG(foldr_func, args) {
    Thunk *f = args[0], *acc = args[1], *l = args[2];
    caseof(l) {
        pat(Nil): {
            return acc;
        }
        pat(Cons): {
            Thunk *x = get(l, 0), *xs = get(l, 1);
            return mulap(f, 2,
                         x,
                         mulap(foldr(), f, acc, xs));
        }
    }
}

GETFUNC(foldr, foldr_func, 3)

