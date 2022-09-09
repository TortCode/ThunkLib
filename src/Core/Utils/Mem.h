#ifndef THUNKLIB__CORE__UTILS__MEM_H
#define THUNKLIB__CORE__UTILS__MEM_H

#include "Lang.h"

#define DECLPTR(type, var) \
type* var = (type*) malloc(sizeof (type))

#define TFREE(var) \
free((qt(MThunk)*)var)

/* MEMORY MANAGEMENT
 * Use instead of directly calling reference counter functions
 */
#define wparam_nonull(thunk,body)  \
qt(Incref)(thunk);          \
body                        \
qt(Decref)(thunk);

#define wparam(thunk,body)  \
wparam_nonull(thunk,body)   \
thunk = NULL;

#define wparamas(thunk, def, body)  \
{                                   \
Thunk* thunk = def                  \
wparam_nonull(thunk, body)          \
}

#define mut_cast(froz) ((qt(MThunk)*) froz)

#endif
