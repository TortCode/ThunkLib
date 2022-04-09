#include "Thunk.h"
#include <stdarg.h>
#include <stdio.h>

Thunk **ThunkList_FromVA(Size len, ...) {
    WVARARGS(list,len,
        Thunk **arr = malloc(len * sizeof (Thunk*));
        for (Size i = 0; i < len; ++i) {
            Thunk_Incref(arr[i] = va_arg(list, Thunk*));
        }
    )
    return arr;
}

Thunk **ThunkList_FromVAList(Size len, va_list list) {
    Thunk **arr = malloc(len * sizeof (Thunk*));
    for (Size i = 0; i < len; ++i) {
        Thunk_Incref(arr[i] = va_arg(list, Thunk*));
    }
    return arr;
}

static void Thunk_DestroyArgs(Thunk *t) {
    if (!t->_args) return;
	for (Size i = 0; i < t->_argc; i++)
		Thunk_Decref(t->_args[i]);
	TFREE(t->_args);
}

static void Value_DecrefFields(Value *v) {
	if (!v->_fields) return;
	for (Size i = 0; i < v->_fieldc; i++)
		Thunk_Decref(v->_fields[i]);
	TFREE(v->_fields);
}

extern inline void Thunk_Incref(Thunk *t) {
	++TOMUT(t)->_refct;
	#ifdef REF_DEBUG
		printf("INCREF @ %d New Count: %d \n", (int) t, t->_refct);
	#endif
}

extern inline void Thunk_Decref(Thunk *t) {
	#ifdef REF_DEBUG
		printf("DECREF @ %d New Count: %d \n", (int) t, t->_refct-1);
	#endif
	//printf("DECREF CALLED: %d \n",(int) t % 1000);
	//printf("%d\n", t->_refct-1);
	if (--TOMUT(t)->_refct == 0) {

		if (t->_primal) {
            Value_DecrefFields(&(t->_val));
		}
        else {
            Thunk_DestroyArgs(t);
        }
		TFREE(t);
	}
}

static Thunk *Thunk_MoveResult(Thunk *dest, Thunk *src) {
#define md (TOMUT(dest))
#define ms (TOMUT(src))
    if (md->_primal = ms->_primal) {
        md->_val = ms->_val;
    }
    else {
        // copy excess of old args
        Size xsargc = -dest->_arity;
        ms->_args = realloc(TOMUT(src->_args), (src->_argc + xsargc) * sizeof(Thunk*));
        for (Size i = 0; i < xsargc; i++) {
            // Need to balance with Thunk_Decref from old arg destruction
            Thunk_Incref(ms->_args[src->_argc + i] = md->_args[dest->_argc - xsargc + i]);
        }
        md->_arity = src->_arity - xsargc;
        md->_func = src->_func;
        // Destroy old args and Move in new args
        Thunk_DestroyArgs(dest);
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
		return Eval(Thunk_MoveResult(t, t->_func(t->_args)));
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
		app->_args = malloc(app->_argc * sizeof(Thunk*));
		//move args in
		for (Size i = 0; i < f_argc; i++) {
			app->_args[i] = f->_args[i];
			Thunk_Incref(f->_args[i]);
		}
		app->_args[f_argc] = TOMUT(x);
		Thunk_Incref(x);
	)
	return app;
}

static Thunk *MultiApply_List(Thunk *f, Size len, Thunk **x) {
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
		app->_args = malloc(app->_argc * sizeof(Thunk*));
		//move args in
		for (Size i = 0; i < f_argc; i++) {
			app->_args[i] = f->_args[i];
			Thunk_Incref(f->_args[i]);
		}
		for (Size i = 0; i < len; i++) {
			app->_args[f_argc + i] = x[i];
			Thunk_Incref(x[i]);
		}
	)
	return app;
}

Thunk *MultiApply(Thunk *f, Size len, ...) {
#ifdef NOFUNC_DEBUG
    if (f->_primal) fprintf(stderr, "First Arg of MultiApply must be a function");
#endif
	WVARARGS(list, len,
        Thunk **x = ThunkList_FromVAList(len, list);
		Thunk *rv = MultiApply_List(f, len, x);
	)
	TFREE(x);
	return rv;
}

Thunk *Thunk_WrapFunc(Executor func, Size arity) {
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

Thunk *Thunk_WrapValue(Value *data) {
	DECLPTR(MThunk, primitive);
    primitive->_refct = 0;
    primitive->_primal = true;
	primitive->_val = *data;
	return primitive;
}

Value *Thunk_ExposeValue(Thunk *t) {
	Thunk_Incref(t);
	#ifdef NOVAL_DEBUG
		if (!Eval(t)->_primal) { fprintf(stderr, "Cannot extract from composite"); exit(EXIT_FAILURE); }
	#endif
		Value *v = &(Eval(t)->_val);
	Thunk_Decref(t);
	return v;
}

extern inline Value *Value_Construct(Size tag, Size fieldc, Thunk **fields) {
    DECLPTR(Value, v);
    v->_tag = tag;
    v->_fieldc = fieldc;
    v->_fields = fields;
	return v;
}


