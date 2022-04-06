#include "list.h"
#include "primitive.h"

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
                      mulap(map(), 2, f, get(l, 1)));
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
                return mk(Cons, x, mulap(filter(), 2, p, xs));
            } else
                return mulap(filter(), 2, p, get(l, 1));
        }
    }
}

GETFUNC(filter, filter_func, 2)

static EXEC_SIG(foldl_func, args) {
    Thunk *f = args[0], *acc = args[1], *l = args[2];
    caseof(l) {
        pat(Nil): {
            return acc;
        }
        pat(Cons): {
            Thunk *x = get(l, 0), *xs = get(l, 1);
            return mulap(foldl(), 3, f, mulap(f, 2, acc, x), xs);
        }
    }
}

GETFUNC(foldl, foldl_func, 3)

