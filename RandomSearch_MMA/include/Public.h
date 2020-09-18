

#if !defined(_extern)
////#warning "if !defined(_extern)"
#define PU
#else
#define PU extern
#endif

#include "Define_file_name.h"


#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#define _extern

///  для EVFalg -значения можно задавать глобально - по умолчанию см Public_init.h ///
PU int    Ahrom, Apair, Agen;


