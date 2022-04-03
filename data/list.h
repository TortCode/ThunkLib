#include "thunk.h" 
#include "data_utils.h"

enum List{
    Nil,
    Cons
};

CONSTRUCTOR_DECL(Nil)
CONSTRUCTOR_DECL(Cons)