#pragma once

#include "framwork.h"

/**
 * @brief binary power algorithm.
 * calculate @p __base ^ @p __exp .
 * 
 * @param __base base number
 * @param __exp exponent number
 * @return size_t 
 */
size_t binpower(size_t __base, size_t __exp);

/**
 * @brief binary power algorithm.
 * calculate ( @p __base ^ @p __exp ) % @p __mod .
 * 
 * @param __base base number
 * @param __exp exponent number
 * @param __mod mod number
 * @return size_t 
 */
size_t binpowerWithMod(size_t __base, size_t __exp, size_t __mod);

/**
 * @brief count `1` bit in binary format of @p __u .
 * 
 * @param __u number
 * @return size_t count
 */
size_t bitcount(size_t __u);

/**
 * @brief calculate greatest common divisor(gcd).
 * between @p __x and @p __y .
 * 
 * @param __x 
 * @param __y 
 * @return int gcd
 */
int gcd(int x, int y);

/**
 * @brief calculate \frac{1}{\sqrt{x}}.
 * 
 * @note this is a quick algothrim but not precise.
 * 
 * @param x 
 * @return float \frac{1}{\sqrt{x}}
 */
float q_rsqrt(float x);
