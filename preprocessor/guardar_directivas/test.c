#define X 10
#include "stdio.h"
#ifdef X
int a;
#else
int b;
#endif
#noesidrectiva
#include <stdlib.h>
#ifndef Y
int c;
#endif