#include "Thunk.h"
#include <stdarg.h>
#include <stdio.h>

#define namespace THUNKNS

qThunk **q(List_FromVA)(qSize len, ...) {
    WVARARGS(list, len,
        qThunk **arr = (qThunk **) malloc(len * sizeof(qThunk *));
        for (qSize i = 0; i < len; ++i) {
            q(Incref)(arr[i] = va_arg(list, qThunk * ));
        }
    )
    return arr;
}

qThunk **q(ListFromVAList)(qSize len, va_list list) {
    qThunk **arr = (qThunk **) malloc(len * sizeof(qThunk *));
    for (qSize i = 0; i < len; ++i) {
        q(Incref)(arr[i] = va_arg(list, qThunk*));
    }
    return arr;
}

static void DestroyArgs(qThunk *t) {
    if (!t->_args) return;
    for (qSize i = 0; i < t->_argc; i++)
        q(Decref)(t->_args[i]);
    TFREE(t->_args);
}

static void DecrefFields(qValue *v) {
    if (!v->_fields) return;
    for (qSize i = 0; i < v->_fieldc; i++)
        q(Decref)(v->_fields[i]);
    TFREE(v->_fields);
}

void q(Incref)(qThunk *t) {
    ++TOMUT(t)->_refct;
#ifdef REF_DEBUG
    printf("INCREF @ %d New Count: %d \n", (int) t, t->_refct);
#endif
}

void q(Decref)(qThunk *t) {
#ifdef REF_DEBUG
    printf("DECREF @ %d New Count: %d \n", (int) t, t->_refct-1);
#endif
    //printf("DECREF CALLED: %d \n",(int) t % 1000);
    //printf("%d\n", t->_refct-1);
    if (--TOMUT(t)->_refct == 0) {

        if (t->_primal) {
            DecrefFields(&(TOMUT(t)->_val));
        } else {
            DestroyArgs(t);
        }
        TFREE(t);
    }
}

static qThunk *MoveResult(qThunk *dest, qThunk *src) {
#define md (TOMUT(dest))
#define ms (TOMUT(src))
    if (md->_primal = ms->_primal) {
        md->_val = ms->_val;
    } else {
        // copy excess of old args
        qSize xsargc = -dest->_arity;
        ms->_args = (qThunk **) realloc(TOMUT(src->_args), (src->_argc + xsargc) * sizeof(qThunk *));
        for (qSize i = 0; i < xsargc; i++) {
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

qThunk *q(Eval)(qThunk *t) {
    if (t->_primal) return t;
    else if (t->_arity <= 0) {
        return q(Eval)(MoveResult(t, t->_func(t->_args)));
    }
    return t;
}

qThunk *q(Apply)(qThunk *f, qThunk *x) {
#ifdef ARITY_DEBUG
    if (f->_primal) { fprintf(stderr, "Cannot apply to primitive\n"); exit(EXIT_FAILURE); }
#endif
    wparam(f,
    //memory management
        DECLPTR(q(MThunk), app);
           app->_refct = 0;
           app->_primal = false;
           //copy func ptr
           app->_func = f->_func;
           //adjust arity/argc info
           qSize f_argc = f->_argc;
           app->_argc = f_argc + 1;
           app->_arity = f->_arity - 1;
           app->_args = (qThunk **) malloc(app->_argc * sizeof(qThunk *));
           //move args in
           for (qSize i = 0; i < f_argc; i++) {
               app->_args[i] = f->_args[i];
               q(Incref)(f->_args[i]);
           }
           app->_args[f_argc] = TOMUT(x);
           q(Incref)(x);
    )
    return app;
}

static qThunk *MultiApplyFromVAList(qThunk *f, qSize len, qThunk **x) {
#ifdef NOFUNC_DEBUG
    if (f->_primal) { fprintf(stderr, "Cannot apply to primitive\n"); exit(EXIT_FAILURE); }
#endif
    wparam(f,
    //memory management
           DECLPTR(q(MThunk), app);
           app->_refct = 0;
           app->_primal = false;
           //copy func ptr
           app->_func = f->_func;
           //adjust arity/argc info
           qSize f_argc = f->_argc;
           app->_argc = f_argc + len;
           app->_arity = f->_arity - len;
           app->_args = (qThunk **) malloc(app->_argc * sizeof(qThunk *));
           //move args in
           for (qSize i = 0; i < f_argc; i++) {
               app->_args[i] = f->_args[i];
               q(Incref)(f->_args[i]);
           }
           for (qSize i = 0; i < len; i++) {
               app->_args[f_argc + i] = x[i];
               q(Incref)(x[i]);
           }
    )
    return app;
}

qThunk *q(MultiApply)(qThunk *f, qSize len, ...) {
#ifdef NOFUNC_DEBUG
    if (f->_primal) fprintf(stderr, "First Arg of MultiApply must be a function");
#endif
    WVARARGS(list, len,
        qThunk **x = q(ListFromVAList)(len, list);
        qThunk *rv = MultiApplyFromVAList(f, len, x);
    )
    TFREE(x);
    return rv;
}

qThunk *q(WrapFunc)(q(Executor) func, qDiff arity) {
    DECLPTR(q(MThunk), caller);
    caller->_refct = 0;
#ifdef ARITY_DEBUG
    if (arity < 1) fprintf(stderr, "Arity of function cannot be 0\n");
#endif
    caller->_primal = false;
    caller->_arity = arity;
    caller->_func = func;
    caller->_argc = 0;
    caller->_args = NULL;
    return caller;
}

qThunk *q(WrapValue)(qValue *data) {
    DECLPTR(q(MThunk), primitive);
    primitive->_refct = 0;
    primitive->_primal = true;
    primitive->_val = *data;
    return primitive;
}

qValue *q(ExposeValue)(qThunk *t) {
    q(Incref)(t);
#ifdef NOVAL_DEBUG
    if (!Eval(t)->_primal) { fprintf(stderr, "Cannot extract from composite"); exit(EXIT_FAILURE); }
#endif
    qValue *v = &(TOMUT(q(Eval)(t))->_val);
    q(Decref)(t);
    return v;
}

qValue *q(Construct)(qSize tag, qSize fieldc, qThunk **fields) {
    DECLPTR(qValue, v);
    v->_tag = tag;
    v->_fieldc = fieldc;
    v->_fields = fields;
    return v;
}

#undef namespace

