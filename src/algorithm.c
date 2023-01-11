#include "algorithm.h"

int binpower(int __base, size_t __exp) {
    int __result = 1;
    while (__exp != 0) {
        if (__exp & 1) 
            __result *= __base;
        __base *= __base;
        __exp >>= 1;
    }
    return __result;
}