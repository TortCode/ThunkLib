#include <stdio.h>
#include "thunk.h"
#include "primitive.h"


int main(void) {
	Thunk* sum = addI();
	Thunk* neg = negI();
	int i = 0;
	int j = 0;
    scanf("%d", &i);
    scanf("%d", &j);
	Thunk* in = ap(sum, wrap(Int32, i));
	Thunk* is = ap(in, wrap(Int32, j));
	Thunk* n = ap(neg, is);
	printf("%d \n", expose(Int32, n));
	return 0;
}