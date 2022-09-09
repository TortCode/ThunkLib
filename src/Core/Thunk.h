#ifndef THUNKLIB__CORE__THUNK_H
#define THUNKLIB__CORE__THUNK_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include "Utils/idx.h"

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

typedef UInt64 Size;
typedef Int64 Diff;
typedef const struct q(thunk_t) Thunk;
typedef struct q(thunk_t) q(MThunk);

typedef Thunk *(*q(Executor))(Thunk **);

typedef struct q(value_t) Value;

/* ALGEBRAIC DATATYPE Value
 * Represents a combination of Product(tuple) and Sum(tagged union) types
 * All ADTs normalize to a Sum of Product types, encoded here as a tagged array of Thunks
 */
struct Value;

/* ALMIGHTY THUNK
 * A structure lazily representing partially applied functions
 * When evaluated at full arity, they move assign the result to themselves
 * Primitive thunks contain a constant value of an Algebraic DataType
 */
struct Thunk;


/* REFERENCE INCREMENT
 * reference count set to 0 when Thunk is constructed
 * Thunk_Incref is called when Thunk references are copied as a local ptr
 * Incref should also be called at the beginning of a function with any Thunk parameters
 * EXCEPT if that function is part of a Thunk, since a Thunk manages its own arguments
 */
void q(Incref)(Thunk *);

/* REFERENCE DECREMENT
 * call Decref after a thunk is no longer needed for computation
 * on local ptrs and parameters of a function
 * DO NOT CALL on thunks returned from function
 * EXCEPT if that function is part of a Thunk, since a Thunk manages its own arguments
 */
void q(Decref)(Thunk *);

/* THUNK CREATOR
 * f: function
 * x: term
 * returns f partially applied to x
 */
Thunk *q(Apply)(Thunk *f, Thunk *x);

#define ap qt(Apply)

/* THUNK CREATOR
 * f: function
 * len: length of x
 * x: array terms
 * returns f partially applied to all arguments in x
 */
Thunk *q(MultiApply)(Thunk *f, Size len, ...);

Thunk *q(Eval)(Thunk *);

/* THUNK CREATOR
 * arity: number of arguments that func requires
 * func: thunk-valued function that accepts array of thunks
 * returns func wrapped in a thunk
 */
Thunk *q(WrapFunc)(q(Executor), Diff);

/* THUNK CREATOR & VALUE BOXER
 * Data: algebraic datatype value
 * returns Data wrapped in a thunk
 */
Thunk *q(WrapValue)(Value *);

/* VALUE UNBOXER
 * t: a thunk that can evaluate to a primal
 * returns the value residing in t
 */
Value *q(ExposeValue)(Thunk *);

Thunk **q(List_FromVA)(Size len, ...);

Thunk **q(ListFromVAList)(Size len, va_list list);

Value *q(Construct)(Size tag, Size fieldc, Thunk **fields);

Size q(Tag)(Value *v);

Thunk *q(Fields)(Value *v, Size i);

#undef namespace
#endif
