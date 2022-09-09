#include <stdio.h>
#include "Core/idx.h"
#include "Data/List.h"


#define offset(type, field) &(((type*)0)->field)

int main(void) {
    FILE *p;
    puts(STR(MG_INFIX));
    Thunk* neg = qu(OPERNS, ainvI)();
	int i = 0;
	int j = 0;
    scanf("%d", &i);
    scanf("%d", &j);
#define list__ qu(Data, List)
    Thunk* sum = qu(OPERNS, addI)();
    Thunk* l = mk(list__, Cons,wrap(Int32, i), mk(list__, Cons, wrap(Int32, j), mk(list__, Nil)));
    Thunk* esum = mlp(qu(list__,foldl()), sum, wrap(Int32, 0), l);
	Thunk* in = ap(sum, wrap(Int32, i));
	Thunk* is = ap(in, wrap(Int32, j));
	Thunk* n = ap(neg, is);
	printf("%d \n", expose(Int32, esum));
	return 0;
}
