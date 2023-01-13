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

size_t bitcount(size_t __u) {
    size_t __result = 0;
    while (__u != 0) {
        __u &= (__u - 1);
        ++__result;
    }
    return __result;
}

int gcd(int x, int y) {   
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunsequenced"
    while(y ^= x ^= y ^= x %= y);
    #pragma GCC diagnostic pop
    /**
     * express parse from right to left.
     * so: x' = x % y
     *     y' = y ^ (x % y)
     *     x' = (x % y) ^ y ^ (x % y) = y
     *     y' = y ^ (x % y) ^ y = x % y
     * finally:
     *     x' = y
     *     y' = x % y
     */
    return x;
}