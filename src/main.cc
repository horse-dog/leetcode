#include "rbtree.h"
#include "algorithm.h"

int main(int argc, const char* argv[]) {

    printf("binpow(%d, %d) = %zu\n", 2, 5, binpower(2, 5));

    printf("binpowerWithMod(%d, %d, %d) = %zu\n", 3, 1000, (int)(1e9+7),
            binpowerWithMod(3, 1000, (int)(1e9+7)));

    printf("bitcount(%d) = %zu\n", 0b11010110, bitcount(0b11010110));

    printf("gcd(%d, %d) = %d\n", 24, 18, gcd(24, 18));

    rbtree<int> rbt;

    int a[] = {5, 15, 4, 2, 8, 3, 14, 9, 1, 12};
    for (int i = 0; i < 10; i++) 
        rbt.insert_equal(a[i]);

    rbt.disp();

    std::cout << rbt << std::endl;
    
    return 0;
}