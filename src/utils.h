#define PP_NARG(...) \
         PP_NARG_(__VA_ARGS__,PP_RSEQ_N)
#define PP_NARG_(...) \
         PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
#define PP_RSEQ_N \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0

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

#define mk(con, ...) TMK(con)(ThunkList_FromVA(PP_NARG(__VA_ARGS__) __VA_OPT__(,) __VA_ARGS__))

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

