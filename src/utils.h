#ifndef UTILS_H
#define UTILS_H

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
Thunk_Decref(thunk); \
thunk = NULL;

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

#define GETFUNC(name, func, arity) \
Thunk* name() { \
    static Thunk* f = NULL; \
    if (!f) f = Thunk_WrapFunc(func, arity); \
    return f; \
}
#define GETFUNC_DECL(name) Thunk* name();

#define TMK(val) Make ## val

#define CON2TAG(con) __ ## con

#define BK_FUNC(f) __ ## f

#define FUNC_DEF(name, arity, args, body)   \
static EXEC_SIG(BK_FUNC(name), args) {               \
    body                                    \
}                                           \
GETFUNC(name, BK_FUNC(name), arity)

#define caseof(t) switch (tag(t))

#define pat(con) case CON2TAG(con)

#define tag(t) Eval(t)->_val->_tag

#define CONSTRUCTOR_DECL(val) Thunk *TMK(val)(Thunk **args);

#define CONSTRUCTOR(con, len)                   \
Thunk *TMK(con)(Thunk **args) { return Thunk_WrapValue(Value_Construct(CON2TAG(con), len, args)); }

#define mk(con, argc, ...) TMK(con)(ThunkList_FromVA(argc __VA_OPT__(,) __VA_ARGS__))

#define ACCESSOR_DECL(name) inline Thunk *Get ## name(Value*);
#define ACCESSOR(name, idx) \
extern inline Thunk *Get ## name(Value *v) { \
    return v->fields[CON2TAG(idx)]; \
}

#define mkstruct(tag, fieldc, ...) Thunk_WrapValue(Value_Construct(tag, CON2TAG(fieldc), __VA_ARGS__))

#ifdef DEBUG
#define get(thunk, idx) (!((thunk)->_val)? fprinf(stderr, "%s not fully evaluated yet!", # thunk) : (thunk)->_val->_fields[CON2TAG(idx)])
#else
#define get(thunk, idx) ((thunk)->_val->_fields[idx])
#endif

#endif

