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

#define MANGLE_ADT(type) __ ## type ## _type

#define data(type) enum struct MANGLE_ADT(type) : THUNK_NAMESPACE::Size

#define con_as_tag(con) __ ## con ## _tag

#define BACKING_FUNC(f) __ ## f ## _func

#define EXECUTOR_SIG(name, args) THUNK_NAMESPACE::Thunk *name(THUNK_NAMESPACE::Thunk **args)

#define define_func(name, arity, args, ...)   \
static EXECUTOR_SIG(BACKING_FUNC(name), args) \
{                                             \
    __VA_ARGS__                               \
}                                             \
GETFUNC(name, BACKING_FUNC(name), arity);

#define caseof(t) switch (tag(t))

#define pat(type,con) case (THUNK_NAMESPACE::Size) MANGLE_ADT(type)::con_as_tag(con)

#define tag(t) THUNK_NAMESPACE::Eval(t)->_val._tag

#define CONSTRUCTOR_DECL(val) EXECUTOR_SIG(MK_THUNK(val), args)

#define CONSTRUCTOR(type, con, len)                   \
CONSTRUCTOR_DECL(con) { return THUNK_NAMESPACE::WrapValue(THUNK_NAMESPACE::Construct((THUNK_NAMESPACE::Size)MANGLE_ADT(type)::con_as_tag(con), len, args)); }

#define mk(con, ...) MK_THUNK(con)(THUNK_NAMESPACE::List_FromVA(PP_NARG(__VA_ARGS__) __VA_OPT__(,) __VA_ARGS__))

#define opap(x, f, ...) mulap(f, x, __VA_ARGS__)

#define mulap(f, ...) THUNK_NAMESPACE::MultiApply(f, PP_NARG(__VA_ARGS__) __VA_OPT__(,) __VA_ARGS__)

#define ACCESSOR_DECL(name) THUNK_NAMESPACE::Thunk *Get ## name(THUNK_NAMESPACE::Value*);
#define ACCESSOR(type, name, idx) \
THUNK_NAMESPACE::Thunk *Get ## name(THUNK_NAMESPACE::Value *v) { \
    return v->fields[MANGLE_ADT(type)::con_as_tag(idx)]; \
}

#define mkstruct(tag, fieldc, ...) THUNK_NAMESPACE::WrapValue(THUNK_NAMESPACE::Construct(tag, fieldc, __VA_ARGS__))

#ifdef DEBUG
#define get(thunk, idx) (!((thunk)->_val)? fprintf(stderr, "%s not fully evaluated yet!", # thunk) : (thunk)->_val._fields[idx])
#else
#define get(thunk, idx) ((thunk)->_val._fields[idx])
#endif

/* argument counting macro
 * usage: PP_NARG(varargs) for up to 127 varargs
 */
#define PP_NARG(...) \
         PP_NARG_(__VA_ARGS__ __VA_OPT__(,) PP_RSEQ_N)
#define PP_NARG_(...) \
         PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
           _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10, \
          _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
          _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
          _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
          _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
          _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
          _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, \
          _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, \
          _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, \
          _91, _92, _93, _94, _95, _96, _97, _98, _99,_100, \
         _101,_102,_103,_104,_105,_106,_107,_108,_109,_110, \
         _111,_112,_113,_114,_115,_116,_117,_118,_119,_120, \
         _121,_122,_123,_124,_125,_126,_127,N,...) N
#define PP_RSEQ_N \
                 127,126,125,124,123,122,121,120, \
         119,118,117,116,115,114,113,112,111,110, \
         109,108,107,106,105,104,103,102,101,100, \
          99, 98, 97, 96, 95, 94, 93, 92, 91, 90, \
          89, 88, 87, 86, 85, 84, 83, 82, 81, 80, \
          79, 78, 77, 76, 75, 74, 73, 72, 71, 70, \
          69, 68, 67, 66, 65, 64, 63, 62, 61, 60, \
          59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
          49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
          39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
          29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
          19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
           9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#endif