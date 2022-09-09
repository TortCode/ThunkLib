#ifndef THUNKLIB__CORE__UTILS__LANG_H
#define THUNKLIB__CORE__UTILS__LANG_H

#include "Macro.h"

//region namespaces

/*
 * Magic infix for namespaces
 * Do not try to operate with namespaces manually
 * Lean on the macros provided  below
 */
#define MG_INFIX __

/*
 * Internal representation of namespaces
 */
#define __NAMESPACE__ namespace

/*
 * returns the name created by joining namespace p and c together
 */
#define qu(p,c) CAT(p, CAT(MG_INFIX, c))

/*
 * returns the name qualified by the Thunk namespace
 */
#define qt(x) qu(THUNKNS, x)

/*
 * returns the name qualified by the Primitive namespace
 */
#define qp(x) qu(PRIMNS, x)

/*
 * returns the name qualified by the Operator namespace
 */
#define qo(x) qu(OPERNS, x)

/*
 * returns the name qualified by the current namespace
 */
#define q(x) qu(__NAMESPACE__, x)

//endregion

//region data tags

/*
 * Header for a datatype tag enum declaration
 */
#define data(type) enum q(CAT(type, _adt))

/*
 * Begins a pattern matching statement
 */
#define caseof(t) switch (qt(Tag)(qt(ExposeValue)(qt(Eval)(t))))

/*
 * Denotes a case of pattern matching
 */
#define pat(base, con) case qu(base, tag(con))

/*
 * Denotes a case of pattern matching with a qualified tag
 */
#define qpat(con) case qtag(con)

/*
 * Internal representation of tag
 */
#define tag(con) CAT(con, _tag)

/*
 * Qualified internal representation
 */
#define qtag(con) q(tag(con))

//endregion

/*
 * A special little child
 */
#define WVARARGS(name, lreqvar, body) \
va_list name;                         \
va_start(name, lreqvar);              \
body                                  \
va_end(name);

//region function utils

/*
 * Name of a constructor
 */
#define MK_THUNK(val) val ## _cons

/*
 * Internal representation of a thunk that represent a library function
 */
#define BACKING_FUNC(f) q(CAT(f, _func))

/*
 * Declaration of thunk singleton
 */
#define decl_func(name) Thunk* q(name)()

/*
 * Implementation of thunk singleton
 */
#define GETFUNC(name, func, arity)                      \
decl_func(name)                                         \
{                                                       \
    static Thunk* f = NULL;                            \
    if (!f) qt(Incref)(f = qt(WrapFunc)(func, arity));  \
    return f;                                           \
}

/*
 * Declaration of Executor as defined in Core.Thunk
 */
#define EXECUTOR_SIG(name, args) Thunk *name(Thunk **args)

/*
 * Thunk singleton paired with implementation of its underlying Executor
 */
#define define_func(name, arity, args, ...)   \
static EXECUTOR_SIG(BACKING_FUNC(name), args) \
{                                             \
    __VA_ARGS__                               \
}                                             \
GETFUNC(name, BACKING_FUNC(name), arity)

/*
 * Signature of a constructor
 */
#define CONSTRUCTOR_SIG(val) \
EXECUTOR_SIG(q(MK_THUNK(val)), args)

/*
 * Declaration of constructor Executor and Thunk singleton
 */
#define decl_constructor(val) \
CONSTRUCTOR_SIG(val); \
decl_func(val)

/*
 * Implementation of constructor Executor and Thunk singleton
 */
#define constructor(type, val, len) \
CONSTRUCTOR_SIG(val) { return qt(WrapValue)(qt(Construct)(q(tag(val)), len, args)); } \
GETFUNC(val, q(MK_THUNK(val)), len)

/*
 * Declaration of field accessor
 */
#define decl_accessor(name) Thunk *q(Get ## name)(qt(Value)* v)

/*
 * Implementation of field accessor
 */
#define accessor(type, name, idx) \
inline decl_accessor(name) { \
    return qt(Fields)(v, qtag(idx)); \
}

//endregion

//region utils for construction, access, and application

/*
 * Makes call to a qualified constructor given name and args
 */
#define qmk(con, ...) mk(namespace, con __VA_OPT__(,) __VA_ARGS__)

/*
 * Makes call to a constructor given namespace, name, and args
 */
#define mk(base, con, ...) qu(base, MK_THUNK(con)) (qt(List_FromVA)(PP_NARG(__VA_ARGS__) __VA_OPT__(,) __VA_ARGS__))

/*
 * Makes generic primitive thunk given tag, number of args, and args
 */
#define mkstruct(tag, fieldc, ...) qt(WrapValue)(qt(Construct)(tag, fieldc, __VA_ARGS__))

/*
 * Retrieves some generic field of a Thunk
 */
#define get(thunk, idx) qt(Fields)(qt(ExposeValue)(thunk), idx)

/*
 * Applies second argument to first, as if it were an infix operator
 */
#define opap(x, f, ...) mulap(f, x, __VA_ARGS__)

/*
 * Applies multiple arguments to the function
 */
#define mlp(f, ...) qt(MultiApply)(f, PP_NARG(__VA_ARGS__) __VA_OPT__(,) __VA_ARGS__)

//endregion

#endif
