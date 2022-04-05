#ifndef THUNK_H
#define THUNK_H

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>

#if defined USE_INT64
typedef int64_t Size;
#else 
typedef int32_t Size;
#endif

#define DECLPTR(type, var) \
type* var = malloc(sizeof (type))

#define tfree(var) \
free((MThunk*)var)

/*MEMORY MANAGEMENT
 *Use instead of directly calling reference counter functions
 */
#define wparam(thunk,body) \
Thunk_Incref(thunk); \
body \
Thunk_Decref(thunk); thunk = NULL;

#define wparamas(thunk, def, body) \
Thunk* thunk = def \
wparam(thunk, body)

#define TOMUT(froz) ((MThunk*) froz)
#define ASMUT(mut, froz) \
MThunk* mut = TOMUT(froz)

#define WVARARGS(name, lreqvar, body) \
va_list name; \
va_start(name, lreqvar); \
body \
va_end(name);

struct Value;

/* ALMIGHTY THUNK
 * A structure lazily representing partially applied functions
 * When evaluated at full arity, they move assign the result to themselves
 * Primitive thunks contain a constant value of an Algebraic DataType
 */
typedef struct thunk_t {
	Size _arity;
	Size _refct;
	const struct thunk_t *(*_func)(const struct thunk_t**);
	Size _argc;
	const struct thunk_t **_args;
	struct Value *_val;
} MThunk;
typedef const struct thunk_t Thunk;
typedef Thunk *(*Executor)(Thunk**);
#define EXEC_SIG(name, args) Thunk *name(Thunk **args)

/* ALGEBRAIC DATATYPE Value
 * Represents a combination of Product(tuple) and Sum(tagged union) types
 * All ADTs normalize to a Sum of Product types, encoded here as a tagged array of Thunks
 */
typedef struct Value {
	Size _tag;
	Size _fieldc;
	Thunk **_fields;
} Value;

/* REFERENCE INCREMENT
 * reference count set to 0 when Thunk is constructed
 * Thunk_Incref is called when Thunk references are copied as a local ptr
 * Thunk_Incref should also be called at the beginning of a function with any Thunk parameters
 * EXCEPT if that function is part of a Thunk, since a Thunk manages its own arguments
 */
inline void Thunk_Incref(Thunk*);

/* REFERENCE DECREMENT
 * call Thunk_Decref after a thunk is no longer needed for computation
 * on local ptrs and parameters of a function
 * DO NOT CALL on thunks returned from function
 * EXCEPT if that function is part of a Thunk, since a Thunk manages its own arguments
 */
inline void Thunk_Decref(Thunk*);

/* THUNK CREATOR
 * f: function
 * x: term
 * returns f partially applied to x
 */
Thunk *Apply(Thunk *f, Thunk *x);
#define ap Apply

/* THUNK CREATOR
 * f: function
 * len: length of x
 * x: array terms
 * returns f partially applied to all arguments in x
 */
Thunk *MultiApply(Thunk *f, Size len, ...);
#define mulap MultiApply

Thunk *Eval(Thunk*);

#define tag(t) Eval(t)->_val->_tag

/* THUNK CREATOR
 * arity: number of arguments that func requires
 * func: thunk-valued function that accepts array of thunks
 * returns func wrapped in a thunk
 */
Thunk *Thunk_WrapFunc(Executor, Size);

/* THUNK CREATOR
 * data: algebraic datatype value
 * returns data wrapped in a thunk
 */
Thunk *Thunk_WrapValue(Value*);

Value *Thunk_ExposeValue(Thunk*);

Value *Value_Construct(Size tag, Size fieldc, Thunk **fields);

Thunk **ThunkList_FromVA(Size len, ...);

Thunk **ThunkList_FromVAList(Size len, va_list list);

#endif
