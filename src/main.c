#include <cstdio>
#include "Core.h"
#include "Data/List.h"


#define offset(type, field) &(((type*)0)->field)

int main(void) {
    puts(STR(MG_INFIX));
//	Thunk* neg = negI();
//	int i = 0;
//	int j = 0;
//    scanf("%d", &i);
//    scanf("%d", &j);
//    Thunk* l = mk(Cons,wrap(Int32, i), mk(Cons, wrap(Int32, j), mk(Nil)));
//    Thunk* esum = mulap(foldl(), sum, wrap(Int32, 0), l);
//	Thunk* in = ap(sum, wrap(Int32, i));
//	Thunk* is = ap(in, wrap(Int32, j));
//	Thunk* n = ap(neg, is);
//	printf("%d \n", expose(Int32, esum));
	return 0;
}