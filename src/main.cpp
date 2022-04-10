#include <cstdio>
#include "Base.h"
#include "Data/List.h"

using namespace THUNK_NAMESPACE;
#define offset(type, field) &(((type*)0)->field)

int main(void) {
    puts(COMPILER_ID);
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