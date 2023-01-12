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
