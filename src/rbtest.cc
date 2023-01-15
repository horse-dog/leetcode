#include "rbtree.h"

#define TEST_EPOCH 1000
#define TEST_COUNT 60

int main(int argc, const char* argv[]) {
    rbtree<int> rbt;
    srand(time(NULL));
{
    for (int i = 0; i < TEST_EPOCH; i++) {
        int arr[TEST_COUNT];
        for (int j = 0; j < TEST_COUNT; j++) {
            arr[j] = rand() % TEST_COUNT;
            rbt.insert_unique(arr[j]);
        }
        printf("%6d ", i + 1); rbt.disp();
        printf("%6d ", i + 1); std::cout << rbt << std::endl;
        for (int j = 0; j < TEST_COUNT; j++)
            rbt.erase(arr[j]);
        printf("%6d ", i + 1); rbt.disp();
        printf("%6d ", i + 1); std::cout << rbt << std::endl;
        assert(rbt.size() == 0);
    }
}

    tiny_mempool::instance().report();
    return 0;
}