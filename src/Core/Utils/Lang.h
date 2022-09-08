#ifndef THUNKLIB__CORE__UTILS__LANG_H
#define THUNKLIB__CORE__UTILS__LANG_H


#define MG_INFIX __

/* qu(p,c)
 * returns the name created by joining namespace p and c together
 */
#define qu(p,c) CAT(p, CAT(MG_INFIX, c))
#define CAT(a,b) _CAT(a,b)
#define _CAT(a,b) a ## b

#define __NAMESPACE__ namespace

#define OPERNS qu(Core, Operator)
#define qt(x) qu(THUNKNS, x)
#define qp(x) qu(PRIMNS, x)
#define q(x) qu(__NAMESPACE__, x)
#define qThunk qt(Thunk)

#define WVARARGS(name, lreqvar, body) \
va_list name;                         \
va_start(name, lreqvar);              \
body                                  \
va_end(name);

#define GETFUNC(name, func, arity)                      \
decl_func(name)                                         \
{                                                       \
    static qThunk* f = NULL;                            \
    if (!f) qt(Incref)(f = qt(WrapFunc)(func, arity));  \
    return f;                                           \
}

#define decl_func(name) qThunk* q(name)()

#define MK_THUNK(val) Make ## val

#define data(type) enum q(CAT(type, _adt))

#define tag(con) CAT(con, _tag)

#define qtag(con) q(tag(con))

#define BACKING_FUNC(f) q(CAT(f, _func))

#define EXECUTOR_SIG(name, args) qThunk *name(qThunk **args)

#define define_func(name, arity, args, ...)   \
static EXECUTOR_SIG(BACKING_FUNC(name), args) \
{                                             \
    __VA_ARGS__                               \
}                                             \
GETFUNC(name, BACKING_FUNC(name), arity)

#define caseof(t) switch (qt(Eval)(t)->_val._tag)

#define qpat(con) case qtag(con)

#define pat(base, con) case qu(base, tag(con))

#define decl_constructor(val) EXECUTOR_SIG(q(MK_THUNK(val)), args)

#define constructor(type, con, len)                   \
decl_constructor(con) { return qt(WrapValue)(qt(Construct)(q(tag(con)), len, args)); }

#define qmk(con, ...) mk(namespace, con __VA_OPT__(,) __VA_ARGS__)

#define mk(base, con, ...) qu(base, MK_THUNK(con)) (qt(List_FromVA)(PP_NARG(__VA_ARGS__) __VA_OPT__(,) __VA_ARGS__))

#define opap(x, f, ...) mulap(f, x, __VA_ARGS__)

#define mulap(f, ...) qt(MultiApply)(f, PP_NARG(__VA_ARGS__) __VA_OPT__(,) __VA_ARGS__)

#define decl_accessor(name) qThunk *q(Get ## name)(qt(Value)* v);
#define accessor(type, name, idx) \
decl_accessor(name) { \
    return v->fields[qtag(idx)]; \
}

#define mkstruct(tag, fieldc, ...) qt(WrapValue)(qt(Construct)(tag, fieldc, __VA_ARGS__))

#ifdef DEBUG
#define get(thunk, idx) (!((thunk)->_val)? fprintf(stderr, "%s not fully evaluated yet!", # thunk) : (thunk)->_val._fields[idx])
#else
#define get(thunk, idx) ((thunk)->_val._fields[idx])
#endif



#endif
