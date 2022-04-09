#include "Thunk.h"
#include <stdarg.h>
#include <stdio.h>


namespace Core::Thunks {
    Thunk **List_FromVA(Size len, ...) {
        WVARARGS(list, len,
            Thunk **arr = (Thunk **) malloc(len * sizeof(Thunk *));
            for (Size i = 0; i < len; ++i) {
                Incref(arr[i] = va_arg(list, Thunk * ));
            }
        )
        return arr;
    }

    Thunk **ListFromVAList(Size len, va_list list) {
        Thunk **arr = (Thunk **) malloc(len * sizeof(Thunk *));
        for (Size i = 0; i < len; ++i) {
            Incref(arr[i] = va_arg(list, Thunk*));
        }
        return arr;
    }

    static void DestroyArgs(Thunk *t) {
        if (!t->_args) return;
        for (Size i = 0; i < t->_argc; i++)
            Decref(t->_args[i]);
        TFREE(t->_args);
    }

    static void DecrefFields(Value *v) {
        if (!v->_fields) return;
        for (Size i = 0; i < v->_fieldc; i++)
            Decref(v->_fields[i]);
        TFREE(v->_fields);
    }

    void Incref(Thunk *t) {
        ++TOMUT(t)->_refct;
#ifdef REF_DEBUG
        printf("INCREF @ %d New Count: %d \n", (int) t, t->_refct);
#endif
    }

    void Decref(Thunk *t) {
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

    static Thunk *MoveResult(Thunk *dest, Thunk *src) {
#define md (TOMUT(dest))
#define ms (TOMUT(src))
        if (md->_primal = ms->_primal) {
            md->_val = ms->_val;
        } else {
            // copy excess of old args
            Size xsargc = -dest->_arity;
            ms->_args = (Thunk **) realloc(TOMUT(src->_args), (src->_argc + xsargc) * sizeof(Thunk *));
            for (Size i = 0; i < xsargc; i++) {
                // Need to balance with Decref from old arg destruction
                Incref(ms->_args[src->_argc + i] = md->_args[dest->_argc - xsargc + i]);
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

    Thunk *Eval(Thunk *t) {
        if (t->_primal) return t;
        else if (t->_arity <= 0) {
            return Eval(MoveResult(t, t->_func(t->_args)));
        }
        return t;
    }

    Thunk *Apply(Thunk *f, Thunk *x) {
#ifdef ARITY_DEBUG
        if (f->_primal) { fprintf(stderr, "Cannot apply to primitive\n"); exit(EXIT_FAILURE); }
#endif
        wparam(f,
        //memory management
            DECLPTR(MThunk, app);
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
               for (Size i = 0; i < f_argc; i++) {
                   app->_args[i] = f->_args[i];
                   Incref(f->_args[i]);
               }
               app->_args[f_argc] = TOMUT(x);
               Incref(x);
        )
        return app;
    }

    static Thunk *MultiApplyFromVAList(Thunk *f, Size len, Thunk **x) {
#ifdef NOFUNC_DEBUG
        if (f->_primal) { fprintf(stderr, "Cannot apply to primitive\n"); exit(EXIT_FAILURE); }
#endif
        wparam(f,
        //memory management
               DECLPTR(MThunk, app);
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
               for (Size i = 0; i < f_argc; i++) {
                   app->_args[i] = f->_args[i];
                   Incref(f->_args[i]);
               }
               for (Size i = 0; i < len; i++) {
                   app->_args[f_argc + i] = x[i];
                   Incref(x[i]);
               }
        )
        return app;
    }

    Thunk *MultiApply(Thunk *f, Size len, ...) {
#ifdef NOFUNC_DEBUG
        if (f->_primal) fprintf(stderr, "First Arg of MultiApply must be a function");
#endif
        WVARARGS(list, len,
            Thunk **x = ListFromVAList(len, list);
            Thunk *rv = MultiApplyFromVAList(f, len, x);
        )
        TFREE(x);
        return rv;
    }

    Thunk *WrapFunc(Executor func, Size arity) {
        DECLPTR(MThunk, caller);
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

    Thunk *WrapValue(Value *data) {
        DECLPTR(MThunk, primitive);
        primitive->_refct = 0;
        primitive->_primal = true;
        primitive->_val = *data;
        return primitive;
    }

    Value *ExposeValue(Thunk *t) {
        Incref(t);
#ifdef NOVAL_DEBUG
        if (!Eval(t)->_primal) { fprintf(stderr, "Cannot extract from composite"); exit(EXIT_FAILURE); }
#endif
        Value *v = &(TOMUT(Eval(t))->_val);
        Decref(t);
        return v;
    }

    Value *Construct(Size tag, Size fieldc, Thunk **fields) {
        DECLPTR(Value, v);
        v->_tag = tag;
        v->_fieldc = fieldc;
        v->_fields = fields;
        return v;
    }
}


