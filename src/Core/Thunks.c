#include "Thunks.h"
#include <stdarg.h>
#include <stdio.h>

#define namespace THUNKNS

struct q(value_t)
{
    Size _tag;
    Size _fieldc;
    Thunk **_fields;
};

struct q(thunk_t)
{
    Size _refct;
    Bool _primal;
    union
    {
        struct
        {
            q(Executor) _func;
            Diff _arity;
            Size _argc;
            Thunk **_args;
        };
        Value _val;
    };
};

Thunk **q(ListFromVA)(Size len, ...)
{
    WVARARGS(list, len,
        Thunk **arr = (Thunk **) malloc(len * sizeof(Thunk *));
        for (Size i = 0; i < len; ++i)
        {
            q(Incref)(arr[i] = va_arg(list, Thunk * ));
        }
    )
    return arr;
}

Thunk **q(ListFromVAList)(Size len, va_list list)
{
    Thunk **arr = (Thunk **) malloc(len * sizeof(Thunk *));
    for (Size i = 0; i < len; ++i)
    {
        q(Incref)(arr[i] = va_arg(list, Thunk *));
    }
    return arr;
}

static void DestroyArgs(Thunk *t)
{
    if (!t->_args) return;
    for (Size i = 0; i < t->_argc; i++)
        q(Decref)(t->_args[i]);
    TFREE(t->_args);
}

static void DecrefFields(Value *v)
{
    if (!v->_fields) return;
    for (Size i = 0; i < v->_fieldc; i++)
        q(Decref)(v->_fields[i]);
    TFREE(v->_fields);
}

inline void q(Incref)(Thunk *t)
{
    ++mut_cast(t)->_refct;
#ifdef REF_DEBUG
    printf("INCREF @ %d New Count: %d \n", (int) t, t->_refct);
#endif
}

void q(Decref)(Thunk *t)
{
#ifdef REF_DEBUG
    printf("DECREF @ %d New Count: %d \n", (int) t, t->_refct-1);
#endif
    //printf("DECREF CALLED: %d \n",(int) t % 1000);
    //printf("%d\n", t->_refct-1);
    if (--mut_cast(t)->_refct == 0)
    {
        if (t->_primal)
        {
            DecrefFields(&(mut_cast(t)->_val));
        }
        else
        {
            DestroyArgs(t);
        }
        TFREE(t);
    }
}


static Thunk *MoveResult(Thunk *dest, Thunk *src)
{
#define md mut_cast(dest)
#define ms mut_cast(src)
    md->_primal = ms->_primal;
    if (md->_primal)
    {   // is primitive thunk
        md->_val = ms->_val;
    }
    else
    {
        // copy excess of old args
        Diff xsargc = -dest->_arity;
        ms->_args = (Thunk **) realloc(mut_cast(src->_args), (src->_argc + xsargc) * sizeof(Thunk *));
        for (Size i = 0; i < xsargc; i++)
        {
            // Need to balance with Decref from old arg destruction
            q(Incref)(ms->_args[src->_argc + i] = md->_args[dest->_argc - xsargc + i]);
        }
        md->_arity = src->_arity - xsargc;
        md->_func = src->_func;
        // Destroy old args and Move in new args
        DestroyArgs(dest);
        md->_argc = src->_argc + xsargc;
        md->_args = src->_args;
        // We do not decref new args since they were transferred
    }

    TFREE(src);
    return dest;
}

Thunk *q(Eval)(Thunk *t)
{
    if (t->_primal) return t;
    else if (t->_arity <= 0)
    {
        return q(Eval)(MoveResult(t, t->_func(t->_args)));
    }
    return t;
}

Thunk *q(Apply)(Thunk *f, Thunk *x)
{
#ifdef ARITY_DEBUG
    if (f->_primal) { eprintf"Cannot apply to primitive\n"); exit(EXIT_FAILURE); }
#endif
    wparam(f,
    //memory management
        DECLPTR(q(MThunk), app);
        app->_refct = 0;
        app->_primal = false;

        //copy func ptr
        app->_func = f->_func;
        //adjust arity/argc info
        Size f_argc = f->_argc;
        app->_argc = f_argc + 1;
        app->_arity = f->_arity - 1;
        app->_args = (Thunk **) malloc(app->_argc * sizeof(Thunk *));
        //move args in
        for (Size i = 0; i < f_argc; i++)
        {
            app->_args[i] = f->_args[i];
            q(Incref)(f->_args[i]);
        }
        app->_args[f_argc] = mut_cast(x);
        q(Incref)(x);
    )
    return app;
}

static Thunk *MultiApplyFromArray(Thunk *f, Size len, Thunk **x)
{
#ifdef NOFUNC_DEBUG
    if (f->_primal) { eprintf("Cannot apply to primitive\n"); exit(EXIT_FAILURE); }
#endif
    wparam(f,
    //memory management
        DECLPTR(q(MThunk), app);
        app->_refct = 0;
        app->_primal = false;
        //copy func ptr
        app->_func = f->_func;
        //adjust arity/argc info
        Size f_argc = f->_argc;
        app->_argc = f_argc + len;
        app->_arity = f->_arity - len;
        app->_args = (Thunk **) malloc(app->_argc * sizeof(Thunk *));
        //move args in
        for (Size i = 0; i < f_argc; i++)
        {
            app->_args[i] = f->_args[i];
            q(Incref)(f->_args[i]);
        }
        for (Size i = 0; i < len; i++)
        {
            app->_args[f_argc + i] = x[i];
            q(Incref)(x[i]);
        }
    )
    return app;
}

Thunk *q(MultiApply)(Thunk *f, Size len, ...)
{
#ifdef NOFUNC_DEBUG
    if (f->_primal) {
        eprintf("First Arg of MultiApply must be a function");
        exit(EXIT_FAILURE);
    }
#endif
    WVARARGS(list, len,
        Thunk **x = q(ListFromVAList)(len, list);
        Thunk *rv = MultiApplyFromArray(f, len, x);
    )
    TFREE(x);
    return rv;
}

Thunk *q(WrapFunc)(q(Executor) func, Diff arity)
{
    DECLPTR(q(MThunk), caller);
    caller->_refct = 0;
#ifdef ARITY_DEBUG
    if (arity < 1) {
        eprintf"Arity of function cannot be 0\n");
        exit(EXIT_FAILURE);
    }
#endif
    caller->_primal = false;
    caller->_arity = arity;
    caller->_func = func;
    caller->_argc = 0;
    caller->_args = NULL;
    return caller;
}

Thunk *q(WrapValue)(Value *data)
{
    DECLPTR(q(MThunk), primitive);
    primitive->_refct = 0;
    primitive->_primal = true;
    primitive->_val = *data;
    return primitive;
}

Value *q(ExposeValue)(Thunk *t)
{
    q(Incref)(t);
#ifdef NOVAL_DEBUG
    if (!Eval(t)->_primal) { eprintf("Cannot extract from composite"); exit(EXIT_FAILURE); }
#endif
    Value *v = &(mut_cast(q(Eval)(t))->_val);
    q(Decref)(t);
    return v;
}

Value *q(Construct)(Size tag, Size fieldc, Thunk **fields)
{
    DECLPTR(Value, v);
    v->_tag = tag;
    v->_fieldc = fieldc;
    v->_fields = fields;
    return v;
}

inline Size q(Tag)(Value *v)
{
    return v->_tag;
}

inline Size q(Fieldc)(Value *v)
{
    return v->_fieldc;
}

inline Thunk *q(Fields)(Value *v, Size i)
{
#ifdef ARITY_DEBUG
    if (i >= v->_fieldc) { eprintf("Cannot access outside of the fields"); }
#endif
    return v->_fields[i];
}

#undef namespace

