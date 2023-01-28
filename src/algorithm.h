#pragma once

#include "framwork.h"
#include <vector>

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

/**
 * @brief construct tree using string format as
 * `num({left_subtree}, {right_subtree})`
 * 
 * @param str format string
 * @return TreeNode* 
 */
TreeNode* bracketConstructTree(const char* str);

/**
 * @brief destroy tree
 * 
 * @param node root node
 */
void destroyTree(TreeNode* node);

/**
 * @brief preorder tree
 * 
 * @param node root node
 * @return std::vector<int> 
 */
std::vector<int> DLRTree(TreeNode* node);

/**
 * @brief inorder tree
 * 
 * @param node root node
 * @return std::vector<int> 
 */
std::vector<int> LDRTree(TreeNode* node);

/**
 * @brief postorder tree
 * 
 * @param node root node
 * @return std::vector<int> 
 */
std::vector<int> LRDTree(TreeNode* node);

/**
 * @brief levelorder tree
 * 
 * @param node root node
 * @return std::vector<int> 
 */
std::vector<int> BFSTree(TreeNode* node);
