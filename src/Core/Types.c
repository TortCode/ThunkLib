#include "Types.h"

#define namespace TYPENS

struct q(type_t)
{
    TypeID _id; // 0 if it is a typevar
    union
    {
        Char _typevar;
        struct
        {
            Size _argc;
            Type **_args;
        };
    };
};

typedef struct
{
    Char _map[256];
} Context;

TypeID q(GenerateTypeID)()
{
    static TypeID id = 1;
    return id++;
}

Bool _Equals(Type *a, Type *b, Context *ctx)
{
    // type constructors equality
    if (a->_id != b->_id) return false;
    // if typevar, check they are same letter
    if (!a->_id)
    {

    }
    // check all the args are equal
    if (a->_argc != b->_argc) return false;
    for (Size i = 0; i < a->_argc; ++i)
    {
        if (!_Equals(a->_args[i], b->_args[i], ctx)) return false;
    }

    // everythings good
    return true;
}

Bool q(Equals)(Type *a, Type *b)
{
    _Equals(a, b, (Context *) calloc(sizeof (Context)));
}



#undef namespace
