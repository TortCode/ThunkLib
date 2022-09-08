#include <stdio.h>
#include "Core.h"
#include "Data/List.h"


#define offset(type, field) &(((type*)0)->field)

int main(void) {
    puts(STR(MG_INFIX));
    qThunk* neg = qu(OPERNS, ainvI)();
	int i = 0;
	int j = 0;
    scanf("%d", &i);
    scanf("%d", &j);
#define listns qu(Data, List)
    qThunk* sum = qu(OPERNS, addI);
    qThunk* l = mk(listns, Cons,wrap(Int32, i), mk(listns, Cons, wrap(Int32, j), mk(listns, Nil)));
    qThunk* esum = mulap(qu(listns,foldl()), sum, wrap(Int32, 0), l);
	qThunk* in = ap(sum, wrap(Int32, i));
	qThunk* is = ap(in, wrap(Int32, j));
	qThunk* n = ap(neg, is);
	printf("%d \n", expose(Int32, esum));
	return 0;
}
