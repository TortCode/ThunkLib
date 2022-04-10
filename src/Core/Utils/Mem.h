#ifndef THUNKLIB__CORE__UTILS__MEM_H
#define THUNKLIB__CORE__UTILS__MEM_H

#include "Lang.h"

#define DECLPTR(type, var) \
type* var = (type*) malloc(sizeof (type))

#define TFREE(var) \
free((qt(MThunk)*)var)

/*MEMORY MANAGEMENT
 *Use instead of directly calling reference counter functions
 */
#define wparam(thunk,body) \
qt(Incref)(thunk); \
body \
qt(Decref)(thunk); \
thunk = NULL;

#define wparamas(thunk, def, body) \
qThunk* thunk = def \
wparam(thunk, body)

#define TOMUT(froz) ((qt(MThunk)*) froz)

#define ASMUT(mut, froz) \
qt(MThunk)* mut = TOMUT(froz)

#endif
