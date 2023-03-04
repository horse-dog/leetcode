#include "algorithm.h"

int main(int argc, const char* argv[]) {

    printf("binpow(%d, %d) = %zu\n", 2, 5, binpower(2, 5));

    printf("binpowerWithMod(%d, %d, %d) = %zu\n", 3, 1000, (int)(1e9+7),
            binpowerWithMod(3, 1000, (int)(1e9+7)));

    printf("bitcount(%d) = %zu\n", 0b11010110, bitcount(0b11010110));

    printf("gcd(%d, %d) = %d\n", 24, 18, gcd(24, 18));

    /**
     *        2
     *      /   \
     *     1     3
     *      \   / \
     *       5 4   7
     */
    auto rt = bracketConstructTree("2(1(,5),3(4,7))");
    bracketDispTree(rt);
    dispContainer(DLRTree(rt));
    dispContainer(LDRTree(rt));
    dispContainer(LRDTree(rt));
    dispContainer(BFSTree(rt));
    destroyTree(rt);

    {
        int arr[] {1, 4, 7, 2, 3, 8, 0, 6, 5};
        sort_heap(arr, arr + 9);
        for (auto&& x : arr) std::cout << x << ' ';
        std::cout << std::endl;
    }

    {
        int arr[] {1, 4, 7, 2, 3, 8, 0, 6, 5};
        qsort(arr, 9);
        for (auto&& x : arr) std::cout << x << ' ';
        std::cout << std::endl;
    }

    return 0;
}