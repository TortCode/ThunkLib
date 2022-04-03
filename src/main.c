#include <stdio.h>
#include "thunk.h"
#include "primitive.h"


int main(void) {
	Thunk* sum = Add();
	Thunk* neg = Neg();
	int i = 4;
	int j = 5;
	Thunk* in = Apply(sum, Thunk_WrapInt32(i));
	Thunk* is = Apply(in, Thunk_WrapInt32(j));
	Thunk* n = Apply(neg, is);
	printf("%d \n", Thunk_ExposeInt32(n));
	return 0;
}