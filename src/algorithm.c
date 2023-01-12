#include "algorithm.h"

size_t binpower(size_t __base, size_t __exp) {
    size_t __result = 1;
    while (__exp != 0) {
        if (__exp & 1) 
            __result *= __base;
        __base *= __base;
        __exp >>= 1;
    }
    return __result;
}

size_t binpowerWithMod(size_t __base, size_t __exp, size_t __mod) {
    size_t __result = 1;
    while (__exp != 0) {
        if (__exp & 1)
            __result = (__result * __base) % __mod;
        __base = (__base * __base) % __mod;
        __exp >>= 1;
    }
    return __result;
}