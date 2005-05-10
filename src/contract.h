#ifndef CONTRACT_H
#define CONTRACT_H

#include "std_cassert.h"

extern bool feature_is_implemented;

#define not_implemented assert(feature_is_implemented)

#define implies(x,y) ((x) && !(y))

#define unexecutable(x) (true)

#endif
