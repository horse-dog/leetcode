#pragma once

#include <vector>
#include <iostream>

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
 * @brief calculate bitcount of u is an odd number
 * or even number.
 * 
 * @example oddbitcount(0b0110) is 0
 * @example oddbitcount(0b1011) is 1
 * 
 * @param u 
 * @return int 
 */
int oddbitcount(uint64_t u);

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

template <class _Container>
void dispContainer(const _Container& c) {
    auto it = c.cbegin();
    if (it == c.end()) {
        putchar('\n');
        return;
    }
    std::cout << *it++;
    for (; it != c.end(); ++it)
        std::cout << ", " << *it;
    putchar('\n');
}

/**
 * @brief construct tree using string format as
 * `num({left_subtree}, {right_subtree})`.
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
 * @brief disp tree using bracket fmt.
 * 
 * @param node root node
 */
void bracketDispTree(TreeNode* node);

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

/**
 * @brief make max-heap in range [from, to)
 * 
 * @param from begin pos
 * @param to end pos
 */
void make_heap(int* from, int* to);

/**
 * @brief push last element to heap
 * @note [from, to - 1) is already max-heap,
 * after this algorithm, [from, to) will be
 * a max-heap.
 * 
 * @param from begin pos
 * @param to end pos
 */
void push_heap(int* from, int* to);

/**
 * @brief pop root element to the end,
 * and after this algorithm, [from, to - 1)
 * will maintain a max-heap.
 * 
 * @param from begin pos
 * @param to end pos
 */
void pop_heap(int* from, int* to);

/**
 * @brief delete a element at specified position,
 * and after this algorithm, [from, to - 1)
 * will maintain a max-heap.
 * 
 * @param from begin pos
 * @param to end pos
 * @param index delete index
 */
void del_heap(int* from, int* to, int index);

/**
 * @brief heap sort
 * 
 * @param from begin pos
 * @param to end pos
 */
void sort_heap(int* from, int* to);