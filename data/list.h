#include "thunk.h" 
#include "data_utils.h"

enum List{
    tNil,
    tCons
};

CONSTRUCTOR_DECL(tNil)
CONSTRUCTOR_DECL(tCons)

GETFUNC_DECL(map)

GETFUNC_DECL(filter)

GETFUNC_DECL(foldl)

GETFUNC_DECL(foldr)