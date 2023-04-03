#include <stack>
#include <queue>
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

size_t lowbit(size_t __u) {
    size_t a = __u & ((~__u) + 1);
    return a;
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

float q_rsqrt(float x) {
    long i;
    float x2, y;
    const float threehalfs = 1.5F;
    x2 = x * 0.5F;
    y = x;
    i = *(long*)&y;
    y = 0x5f3759df - ( i >> 1);
    y = *(float*)&i;
    y = y * (threehalfs - (x2 * y * y));
    return y;
}

static TreeNode* __bracketConstructTree(const char*& str) {
    if (*str == '\0' || *str == ',') 
        return nullptr;
    TreeNode* node = 
        new TreeNode {(int)strtol(str, (char**)&str, 10), nullptr, nullptr};
    if (*str == '(') {
        node->left = __bracketConstructTree(++str);
        if (*str == ',')
            node->right = __bracketConstructTree(++str);
        if (*str != ')')
            throw std::runtime_error("fmt string error");
        ++str;  // this is ')'
    } 
    return node;
}

TreeNode* bracketConstructTree(const char* str) {
    TreeNode* root = __bracketConstructTree(str);
    if (*str != '\0') throw std::runtime_error("fmt string error");
    return root;
}

void destroyTree(TreeNode* node) {
    if (node == nullptr) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;    
}

static void __bracketDispTree(TreeNode* node) {
    if (node == nullptr) return;
    printf("%d", node->val);
    if (node->left || node->right) {
        putchar('(');
        __bracketDispTree(node->left);
        if (node->right) putchar(',');
        __bracketDispTree(node->right);            
        putchar(')');
    }
}

void bracketDispTree(TreeNode* node) {
    __bracketDispTree(node);
    putchar('\n');
}

std::vector<int> DLRTree(TreeNode* node) {
    std::stack<TreeNode*> st;
    std::vector<int> res;
    while (!st.empty() || node) {
        while (node) {
            res.push_back(node->val);
            st.push(node);
            node = node->left;
        }
        node = st.top()->right;
        st.pop();
    }
    return res;
}

std::vector<int> LDRTree(TreeNode* node) {
    std::stack<TreeNode*> st;
    std::vector<int> res;
    while (!st.empty() || node) {
        while (node) {
            st.push(node);
            node = node->left;
        }
        node = st.top();
        st.pop();
        res.push_back(node->val);
        node = node->right;
    }
    return res;
}

std::vector<int> LRDTree(TreeNode* node) {
    std::stack<TreeNode*> st;
    std::vector<int> res;
    TreeNode* pre = nullptr;
    while (!st.empty() || node) {
        while (node) {
            st.push(node);
            node = node->left;
        }
        node = st.top();
        if (node->right != nullptr && node->right != pre)
            node = node->right;
        else {
            st.pop();
            res.push_back(node->val);
            pre = node;
            node = nullptr;
        }
    }
    return res;
}

std::vector<int> BFSTree(TreeNode* node) {
    if (node == nullptr) return {};
    std::queue<TreeNode*> qu;
    std::vector<int> res;
    qu.push(node);
    while (!qu.empty()) {
        TreeNode* tmp = qu.front();
        qu.pop();
        res.push_back(tmp->val);
        if (tmp->left)
            qu.push(tmp->left);
        if (tmp->right)
            qu.push(tmp->right);
    }
    return res;
}

static bool siftDown(int* from, int index, int n) {
    int i = index;
    int j = i * 2 + 1; // lchild index
    int tmp = from[i];
    while (j < n) {
        // if rchild is exist and rchild > lchild
        if (j + 1 < n && from[j] < from[j + 1])
            j++;
        if (from[j] < tmp) break;
        from[i] = from[j];
        i = j;
        j = i * 2 + 1;
    }
    from[i] = tmp;
    return i > index;
}

static void siftUp(int* from, int index) {
    if (index <= 0) return;
    int i = (index - 1) / 2; // parent index
    int tmp = from[index];
    while (i >= 0) {
        if (from[index] < tmp) break;
        from[index] = from[i];
        index = i;
        i = (index - 1) / 2;
    }
    from[index] = tmp;
}

void make_heap(int* from, int* to) {
    int n = to - from;
    for (int i = (n - 2) / 2; i >= 0; i--)
        siftDown(from, i, n);
}

void push_heap(int* from, int* to) {
    siftUp(from, to - from - 1);
}

void pop_heap(int* from, int* to) {
    del_heap(from, to, 0);
}

void del_heap(int* from, int* to, int index) {
    int n = to - from;
    if (index < 0 || index >= n) return;
    // now index is in [0, index)
    if (index == n - 1) return;
    std::swap(from[index], *(to - 1));
    if (!siftDown(from, index, n - 1))
        siftUp(from, index);
    return;
}

void sort_heap(int* from, int* to) {
    int n = to - from;
    make_heap(from, to);
    for (int i = n; i > 1; i--)
        pop_heap(from, from + i);
}

void qsort(int* arr, int n) {
    if (n <= 1) return;
    int x = arr[0];
    int l = 0, r = n - 1;
    while (l < r) {
        while (l < r && arr[r] >= x) --r;
        arr[l] = arr[r];
        while (l < r && arr[l] <= x) ++l;
        arr[r] = arr[l];
    }
    arr[l] = x;
    qsort(arr, l);
    qsort(arr + l + 1, n - l - 1);
}
