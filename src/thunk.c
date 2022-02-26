#include "thunk.h"
#include <stdarg.h>
#include <stdio.h>

void Thunk_DestroyArgs(Thunk *t) {
	for (Size i = 0; i < t->_argc; i++)
		Thunk_Decref(t->_args[i]);
	tfree(t->_args);
}

void Value_DestroyFields(Value *v) {
	if (!v->_fields) return;
	for (Size i = 0; i < v->_fieldc; i++)
		Thunk_Decref(v->_fields[i]);
	tfree(v->_fields);
}

extern inline void Thunk_Incref(Thunk *t) {
	asmut(mt, t);
	++mt->_refct;
}

extern inline void Thunk_Decref(Thunk *t) {
	asmut(mt, t);
	//printf("DECREF CALLED: %d \n",(int) t % 1000);
	//printf("%d\n", t->_refct-1);
	if (--mt->_refct == 0) {
		Thunk_DestroyArgs(t);
		if (t->_val) {
			Value_DestroyFields(t->_val);
			free(t->_val);
		}
		tfree(t);
	}
}

/* Only use when dest completely owns src */
void Thunk_Replace(Thunk *dest, Thunk *src) {
	asmut(md, dest);
	asmut(ms, src);
	// copy excess of old args
	Size xsargc = -dest->_arity;
	ms->_args = realloc(tomut(ms->_args), (ms->_argc + xsargc) * sizeof(Thunk*));
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
		declptr(MThunk, app);
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
		app->_args[f_argc] = tomut(x);
		Thunk_Incref(x);
	)
	return app;
}

Thunk *MultiApply(Thunk *f, Size len, Thunk **x) {
#ifdef DEBUG
	if (f->_val) { fprintf(stderr, "Cannot apply to primitive\n"); exit(EXIT_FAILURE); }
#endif
	wparam(f,
		//memory management
		declptr(MThunk, app);
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

Thunk **Thunk_List(Size len, va_list list) {
	Thunk **arr = malloc(len * sizeof (Thunk*));
	for (Size i = 0; i < len; ++i) {
		Thunk_Incref(arr[i] = va_arg(list, Thunk*));
	}
	return arr;
}

Thunk *MultiApply_VA(Thunk *f, Size len, ...) {
	wvarargs(list,len,
		Thunk **x=0;
		Thunk *rv = MultiApply(f, len, x=Thunk_List(len, list));
	)
	tfree(x);
	return rv;
}

Thunk *compose(Thunk *f, Thunk *g, Thunk *x) {
	wparam(f,
	wparam(g,
	wparam(x,
		Thunk *rv = Apply(f, Apply(g, x));
	)))
	return rv;
}

Thunk *Thunk_FuncWrap(Thunk *(*func)(Thunk**), Size arity) {
	declptr(MThunk, caller);
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

Thunk *Thunk_ValueWrap(Value *data) {
	declptr(MThunk, primitive);
	primitive->_refct = 0;
	primitive->_arity = 0;
	primitive->_func = NULL;
	primitive->_argc = 0;
	primitive->_args = NULL;
	primitive->_val = data;
	return primitive;
}

Value *Thunk_ValueExpose(Thunk *t) {
	Thunk_Incref(t);
	#ifdef DEBUG
		if (!Eval(t)->_val) { fprintf(stderr, "Cannot extract from composite"); exit(EXIT_FAILURE); }
	#endif
		Value *v = Eval(t)->_val;
	Thunk_Decref(t);
	return v;
}

Value *Value_Construct(Size tag, Size fieldc,...) {
	wvarargs(list, fieldc,
		declptr(Value, v);
		v->_tag = tag;
		v->_fieldc = fieldc;
		v->_fields = Thunk_List(fieldc, list);
	)
	return v;
}

Value *Value_ConstructVAList(Size tag, Size fieldc, va_list list) {
	declptr(Value, v);
	v->_tag = tag;
	v->_fieldc = fieldc;
	v->_fields = Thunk_List(fieldc, list);
	return v;
}