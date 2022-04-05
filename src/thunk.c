#include "thunk.h"
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
	for (Size i = 0; i < t->_argc; i++)
		Thunk_Decref(t->_args[i]);
	tfree(t->_args);
}

static void Value_DestroyFields(Value *v) {
	if (!v->_fields) return;
	for (Size i = 0; i < v->_fieldc; i++)
		Thunk_Decref(v->_fields[i]);
	tfree(v->_fields);
}

extern inline void Thunk_Incref(Thunk *t) {
	++TOMUT(t)->_refct;
	#ifdef DEBUG
		printf("INCREF @ %d New Count: %d \n", (int) t, t->_refct);
	#endif
}

extern inline void Thunk_Decref(Thunk *t) {
	#ifdef DEBUG
		printf("DECREF @ %d New Count: %d \n", (int) t, t->_refct-1);
	#endif
	//printf("DECREF CALLED: %d \n",(int) t % 1000);
	//printf("%d\n", t->_refct-1);
	if (--TOMUT(t)->_refct == 0) {
		Thunk_DestroyArgs(t);
		if (t->_val) {
			Value_DestroyFields(t->_val);
			free(t->_val);
		}
		tfree(t);
	}
}

/* Only use when dest completely owns src */
static void Thunk_Replace(Thunk *dest, Thunk *src) {
	ASMUT(md, dest);
	ASMUT(ms, src);
	// copy excess of old args
	Size xsargc = -dest->_arity;
	ms->_args = realloc(TOMUT(ms->_args), (ms->_argc + xsargc) * sizeof(Thunk*));
	for (Size i = 0; i < xsargc; i++) {
		// Need to balance with Thunk_Decref from old arg destruction
		Thunk_Incref(ms->_args[ms->_argc + i] = md->_args[md->_argc - xsargc + i]);
	}
	// Function stoof
	md->_arity = ms->_arity - xsargc;
	md->_func = ms->_func;
	// Destroy old args and Move in new args
	Thunk_DestroyArgs(md);
	md->_argc = ms->_argc + xsargc;
	md->_args = ms->_args;
	md->_val = ms->_val;
	// We do not decref new args since they were transfered
	free(ms);
}

Thunk *Eval(Thunk *t) {
	if (t->_val) return t;
	if (t->_arity <= 0) {
		Thunk_Replace(t, t->_func(t->_args));
		return Eval(t);
	}
	return t;
}

Thunk *Apply(Thunk *f, Thunk *x) {
#ifdef DEBUG
	if (f->_val) { fprintf(stderr, "Cannot apply to primitive\n"); exit(EXIT_FAILURE); }
#endif
	wparam(f,
		//memory management
		DECLPTR(MThunk, app);
		app->_refct = 0;
		//copy func ptr
		app->_func = f->_func;
		app->_val = NULL;
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
#ifdef DEBUG
	if (f->_val) { fprintf(stderr, "Cannot apply to primitive\n"); exit(EXIT_FAILURE); }
#endif
	wparam(f,
		//memory management
		DECLPTR(MThunk, app);
		app->_refct = 0;
		//copy func ptr
		app->_func = f->_func;
		app->_val = NULL;
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
	WVARARGS(list, len,
        Thunk **x = ThunkList_FromVAList(len, list);
		Thunk *rv = MultiApply_List(f, len, x);
	)
	tfree(x);
	return rv;
}

Thunk *Thunk_WrapFunc(Thunk *(*func)(Thunk**), Size arity) {
	DECLPTR(MThunk, caller);
	caller->_refct = 0;
#ifdef DEBUG
	if (arity < 1) fprintf(stderr, "Arity of function cannot be 0\n");
#endif
	caller->_arity = arity;
	caller->_func = func;
	caller->_argc = 0;
	caller->_args = NULL;
	caller->_val = NULL;
	return caller;
}

Thunk *Thunk_WrapValue(Value *data) {
	DECLPTR(MThunk, primitive);
	primitive->_refct = 0;
	primitive->_arity = 0;
	primitive->_func = NULL;
	primitive->_argc = 0;
	primitive->_args = NULL;
	primitive->_val = data;
	return primitive;
}

Value *Thunk_ExposeValue(Thunk *t) {
	Thunk_Incref(t);
	#ifdef DEBUG
		if (!Eval(t)->_val) { fprintf(stderr, "Cannot extract from composite"); exit(EXIT_FAILURE); }
	#endif
		Value *v = Eval(t)->_val;
	Thunk_Decref(t);
	return v;
}

Value *Value_Construct(Size tag, Size fieldc, Thunk **fields) {
    DECLPTR(Value, v);
    v->_tag = tag;
    v->_fieldc = fieldc;
    v->_fields = fields;
	return v;
}


