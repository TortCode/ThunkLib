#ifndef THUNKLIB__CORE__THUNK_H
#define THUNKLIB__CORE__THUNK_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include "Utils.h"

typedef int8_t Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;
typedef uint8_t UInt8;
typedef uint16_t UInt16;
typedef uint32_t UInt32;
typedef uint64_t UInt64;
typedef float Float32;
typedef double Float64;
typedef bool Bool;
typedef char Char;


#define namespace qu(Core, Thunk)
#define THUNKNS qu(Core, Thunk)

typedef UInt64 q(Size);
typedef Int64 q(Diff);
typedef const struct q(thunk_t) q(Thunk);
typedef struct q(thunk_t) q(MThunk);

typedef qThunk *(*q(Executor))(qThunk **);

typedef struct q(value_t) q(Value);

#define qValue qt(Value)
#define qSize qt(Size)
#define qDiff qt(Diff)

/* ALGEBRAIC DATATYPE Value
 * Represents a combination of Product(tuple) and Sum(tagged union) types
 * All ADTs normalize to a Sum of Product types, encoded here as a tagged array of Thunks
 */
struct q(value_t)
{
    qSize _tag;
    qSize _fieldc;
    qThunk **_fields;
};

qValue *q(Construct)(qSize tag, qSize fieldc, qThunk **fields);

/* ALMIGHTY THUNK
 * A structure lazily representing partially applied functions
 * When evaluated at full arity, they move assign the result to themselves
 * Primitive thunks contain a constant value of an Algebraic DataType
 */
struct q(thunk_t)
{
    qSize _refct;
    Bool _primal;
    union
    {
        struct
        {
            q(Executor) _func;
            qDiff _arity;
            qSize _argc;
            qThunk **_args;
        };
        qValue _val;
    };

};


/* REFERENCE INCREMENT
 * reference count set to 0 when Thunk is constructed
 * Thunk_Incref is called when Thunk references are copied as a local ptr
 * Incref should also be called at the beginning of a function with any Thunk parameters
 * EXCEPT if that function is part of a Thunk, since a Thunk manages its own arguments
 */
void q(Incref)(qThunk *);

/* REFERENCE DECREMENT
 * call Decref after a thunk is no longer needed for computation
 * on local ptrs and parameters of a function
 * DO NOT CALL on thunks returned from function
 * EXCEPT if that function is part of a Thunk, since a Thunk manages its own arguments
 */
void q(Decref)(qThunk *);

/* THUNK CREATOR
 * f: function
 * x: term
 * returns f partially applied to x
 */
qThunk *q(Apply)(qThunk *f, qThunk *x);

#define ap qt(Apply)

/* THUNK CREATOR
 * f: function
 * len: length of x
 * x: array terms
 * returns f partially applied to all arguments in x
 */
qThunk *q(MultiApply)(qThunk *f, qSize len, ...);

qThunk *q(Eval)(qThunk *);

/* THUNK CREATOR
 * arity: number of arguments that func requires
 * func: thunk-valued function that accepts array of thunks
 * returns func wrapped in a thunk
 */
qThunk *q(WrapFunc)(q(Executor), qDiff);

/* THUNK CREATOR & VALUE BOXER
 * Data: algebraic datatype value
 * returns Data wrapped in a thunk
 */
qThunk *q(WrapValue)(qValue *);

/* VALUE UNBOXER
 * t: a thunk that can evaluate to a primal
 * returns the value residing in t
 */
qValue *q(ExposeValue)(qThunk *);

qThunk **q(List_FromVA)(qSize len, ...);

qThunk **q(ListFromVAList)(qSize len, va_list list);

#undef namespace

#endif
