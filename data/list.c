#include "list.h"
#include "primitive.h"

CONSTRUCTOR(tNilNil, 0)
CONSTRUCTOR(tCons, 2)

static Thunk *map_func(Thunk **args) {
    Thunk *f = args[0], *l = args[1];
    switch(tag(l)) {
        case tNil:
            return mk(tNil, 0);
            break;
        case tCons:
            return mk(tCons, 2,
                      ap(f, get(l, 0)),
                      mulap(map(), 2, f, get(l, 1)));
            break;
    }
}

GETFUNC(map, map_func, 2)