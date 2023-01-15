#include "rbtree.h"
#include "algorithm.h"

int main(int argc, const char* argv[]) {

    printf("binpow(%d, %d) = %zu\n", 2, 5, binpower(2, 5));

    printf("binpowerWithMod(%d, %d, %d) = %zu\n", 3, 1000, (int)(1e9+7),
            binpowerWithMod(3, 1000, (int)(1e9+7)));

    printf("bitcount(%d) = %zu\n", 0b11010110, bitcount(0b11010110));

    printf("gcd(%d, %d) = %d\n", 24, 18, gcd(24, 18));

    rbtree<int> rbt;

    int a[] = {5, 15, 4, 17, 2, 20, 8, 18, 3, 14, 9, 19, 1, 16, 12, 7, 11, 6, 10, 13};
    for (int i = 0; i < 20; i++) {
        rbt.insert_unique(a[i]);
        rbt.disp();
    }

    std::cout << rbt << std::endl;

    for (int i = 0; i < 20; i++) {
        rbt.disp();
        rbt.erase(a[i]);
    }

    std::cout << rbt << std::endl;

    for (int i = 0; i < 20; i++) 
        rbt.insert_unique(a[i]);

    rbtree<int> rbt1;
    rbt1 = rbt;

    rbt.disp(); rbt1.disp();

    rbt1 = std::move(rbt);

    rbt.disp(); rbt1.disp();
    
    return 0;
}