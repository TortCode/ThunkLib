#include <stdio.h>
#include "thunk.h"
#include "primitive.h"

Int32 extracti(Thunk* t) {
	Thunk_Incref(t);
	Int32 i = Thunk_ValueExpose(t)->_tag;
	Thunk_Decref(t);
	return i;
}
float extractf(Thunk* t) {
	Thunk_Incref(t);
	float f = *((float *)&Thunk_ValueExpose(t)->_tag);
	Thunk_Decref(t);
	return f;
}
Thunk *addi(Thunk** args) {
	return Thunk_Int32Wrap(extracti(args[0]) + extracti(args[1]));
}

int main(void) {
	Thunk* sum = Add();
	Thunk* neg = Neg();
	int i = 4;
	int j = 5;
	Thunk* in = Apply(sum, Thunk_Int32Wrap(i));
	Thunk* is = Apply(in, Thunk_Int32Wrap(j));
	Thunk* n = Apply(neg, is);
	printf("%d \n", Thunk_Int32Expose(n));
	return 0;
}