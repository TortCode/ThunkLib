#ifndef THUNKLIB__CORE__UTILS__LANG_H
#define THUNKLIB__CORE__UTILS__LANG_H

#define THUNK_NAMESPACE Core::Thunks
#define PRIMITIVE_NAMESPACE Core::Primitive

#define WVARARGS(name, lreqvar, body) \
va_list name; \
va_start(name, lreqvar); \
body \
va_end(name);

#define GETFUNC(name, func, arity) \
GETFUNC_DECL(name) {              \
    using namespace THUNK_NAMESPACE;             \
    static Thunk* f = NULL; \
    if (!f) Incref(f = WrapFunc(func, arity)); \
    return f; \
}

#define GETFUNC_DECL(name) THUNK_NAMESPACE::Thunk* name()

#define MK_THUNK(val) Make ## val

#define CON2TAG(con) __ ## con ## _tag

#define BACKING_FUNC(f) __ ## f ## _func

#define EXECUTOR_SIG(name, args) THUNK_NAMESPACE::Thunk *name(THUNK_NAMESPACE::Thunk **args)

#define define_func(name, arity, args, body)\
static EXECUTOR_SIG(BACKING_FUNC(name), args) {      \
    body                                    \
}                                           \
GETFUNC(name, BACKING_FUNC(name), arity)

#define caseof(t) switch (tag(t))

#define pat(con) case CON2TAG(con)

#define tag(t) THUNK_NAMESPACE::Eval(t)->_val._tag

#define CONSTRUCTOR_DECL(val) EXECUTOR_SIG(MK_THUNK(val), args)

#define CONSTRUCTOR(con, len)                   \
CONSTRUCTOR_DECL(con) { return THUNK_NAMESPACE::WrapValue(THUNK_NAMESPACE::Construct(CON2TAG(con), len, args)); }

#define mk(con, ...) MK_THUNK(con)(THUNK_NAMESPACE::List_FromVA(PP_NARG(__VA_ARGS__) __VA_OPT__(,) __VA_ARGS__))

#define mulap(f, ...) THUNK_NAMESPACE::MultiApply(f, PP_NARG(__VA_ARGS__) __VA_OPT__(,) __VA_ARGS__)

#define ACCESSOR_DECL(name) inline THUNK_NAMESPACE::Thunk *Get ## name(THUNK_NAMESPACE::Value*);
#define ACCESSOR(name, idx) \
extern inline THUNK_NAMESPACE::Thunk *Get ## name(THUNK_NAMESPACE::Value *v) { \
    return v->fields[CON2TAG(idx)]; \
}

#define mkstruct(tag, fieldc, ...) THUNK_NAMESPACE::WrapValue(THUNK_NAMESPACE::Construct(tag, fieldc, __VA_ARGS__))

#ifdef DEBUG
#define get(thunk, idx) (!((thunk)->_val)? fprintf(stderr, "%s not fully evaluated yet!", # thunk) : (thunk)->_val._fields[idx])
#else
#define get(thunk, idx) ((thunk)->_val._fields[idx])
#endif

#endif
