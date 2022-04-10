#ifndef THUNKLIB__CORE__UTILS__MEM_H
#define THUNKLIB__CORE__UTILS__MEM_H

#define DECLPTR(type, var) \
type* var = (type*) malloc(sizeof (type))

#define TFREE(var) \
free((Core::Thunks::MThunk*)var)

/*MEMORY MANAGEMENT
 *Use instead of directly calling reference counter functions
 */
#define wparam(thunk,body) \
Core::Thunks::Incref(thunk); \
body \
Core::Thunks::Decref(thunk); \
thunk = NULL;

#define wparamas(thunk, def, body) \
Core::Thunks::Thunk* thunk = def \
wparam(thunk, body)

#define TOMUT(froz) ((Core::Thunks::MThunk*) froz)

#define ASMUT(mut, froz) \
Core::Thunks::MThunk* mut = TOMUT(froz)

#endif
