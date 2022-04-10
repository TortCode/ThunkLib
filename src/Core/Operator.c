#include "Primitive.h"
#include "Operator.h"

#define namespace OPERNS

//Generalized definition of a unary operator
#define UNOP(name, type, op) \
    define_func(name, 1, args, return wrap(type, (op expose(type,args[0]))); )
//Generalized definition of a binary operator
#define BINOP(name, type, op) \
    define_func(name, 2, args, return wrap(type, (expose(type, args[0]) op expose(type, args[1]))); )                              \

//Gives variations of operator for each type
#define OP_FORALLNUMS(name, op, kind) \
    kind(name ## B, Int8, op)             \
    kind(name ## S, Int16, op)            \
    kind(name ## I, Int32, op)            \
    kind(name ## L, Int64, op)            \
    kind(name ## F, Float32, op)          \
    kind(name ## D, Float64, op)

OP_FORALLNUMS(ainv, -, UNOP)
UNOP(neg, Bool, !)

OP_FORALLNUMS(add, +, BINOP)
OP_FORALLNUMS(sub, -, BINOP)
OP_FORALLNUMS(mul, *, BINOP)
OP_FORALLNUMS(div, /, BINOP)
BINOP(modI, Int32, %) BINOP(modL, Int64, %)

BINOP(conj, Bool, &&)
BINOP(disj, Bool, ||)
BINOP(eq, Bool, ==)
BINOP(neq, Bool, !=)
BINOP(lt, Bool, <)
BINOP(lte, Bool, <=)
BINOP(gt, Bool, >)
BINOP(gte, Bool, >=)

define_func(compose, 3, args, return qt(Apply)(args[0], Apply(args[1], args[2])); )
define_func(app, 2, args, return qt(Apply)(args[0], args[1]); )

#undef namespace

