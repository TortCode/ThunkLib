#ifndef THUNKLIB__CORE__UTILS__LANG_H
#define THUNKLIB__CORE__UTILS__LANG_H

#define WVARARGS(name, lreqvar, body) \
va_list name; \
va_start(name, lreqvar); \
body \
va_end(name);

#define GETFUNC(name, func, arity) \
Thunk* name() { \
    static Thunk* f = NULL; \
    if (!f) Thunk_Incref(f = Thunk_WrapFunc(func, arity)); \
    return f; \
}
#define GETFUNC_DECL(name) Thunk* name();

#define TMK(val) Make ## val

#define CON2TAG(con) __ ## con ## _tag

#define BK_FUNC(f) __ ## f ## _func

#define define_func(name, arity, args, body)   \
static EXEC_SIG(BK_FUNC(name), args) {               \
    body                                    \
}                                           \
GETFUNC(name, BK_FUNC(name), arity)

#define caseof(t) switch (tag(t))

#define pat(con) case CON2TAG(con)

#define tag(t) Eval(t)->_val._tag

#define CONSTRUCTOR_DECL(val) Thunk *TMK(val)(Thunk **args);

#define CONSTRUCTOR(con, len)                   \
Thunk *TMK(con)(Thunk **args) { return Thunk_WrapValue(Value_Construct(CON2TAG(con), len, args)); }

#define mk(con, ...) TMK(con)(ThunkList_FromVA(PP_NARG(__VA_ARGS__) __VA_OPT__(,) __VA_ARGS__))

#define mulap(f, ...) MultiApply(f, PP_NARG(__VA_ARGS__) __VA_OPT__(,) __VA_ARGS__)

#define ACCESSOR_DECL(name) inline Thunk *Get ## name(Value*);
#define ACCESSOR(name, idx) \
extern inline Thunk *Get ## name(Value *v) { \
    return v->fields[CON2TAG(idx)]; \
}

#define mkstruct(tag, fieldc, ...) Thunk_WrapValue(Value_Construct(tag, fieldc, __VA_ARGS__))

#ifdef DEBUG
#define get(thunk, idx) (!((thunk)->_val)? fprintf(stderr, "%s not fully evaluated yet!", # thunk) : (thunk)->_val._fields[idx])
#else
#define get(thunk, idx) ((thunk)->_val._fields[idx])
#endif

#endif
