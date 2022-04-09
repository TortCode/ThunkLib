#include <cstdio>
#include "Core.h"
#include "Data/List.h"

using namespace THUNK_NAMESPACE;
#define offset(type, field) &(((type*)0)->field)

int main(void) {
    printf("%ld \n", sizeof(int));
    printf("refct: %p \n", offset(Thunk, _refct));
    printf("primal: %p \n", offset(Thunk, _primal));
    printf("func: %p \n", offset(Thunk, _func));
    printf("arity: %p \n", offset(Thunk, _arity));
    printf("argc: %p \n", offset(Thunk, _argc));
    printf("args: %p \n", offset(Thunk, _args));
    printf("val: %p \n", offset(Thunk, _val));
    printf("%ld \n", sizeof(Thunk));
    Thunk *t= wrap(Int32, 1);

    Construct(0,0,0);
Thunk* sum = Core::Primitive::addI();
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