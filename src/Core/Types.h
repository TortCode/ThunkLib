#ifndef THUNKLIB__CORE__TYPES_H
#define THUNKLIB__CORE__TYPES_H

#include "Thunks.h"

#define namespace qu(Core, Types)
#define TYPENS qu(Core, Types)

typedef const struct q(type_t) Type;
typedef UInt32 TypeID;

struct Type;

#undef namespace

#endif
