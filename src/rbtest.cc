#include "rbtree.h"
#include <thread>
#include <vector>

using namespace std::chrono;

#define TEST_EPOCH 100
#define TEST_COUNT 50
#define TEST_THREAD 20

static void test_rbtree() {
    rbtree<int> rbt;
    for (int i = 0; i < TEST_EPOCH; i++) {
        int arr[TEST_COUNT];
        for (int j = 0; j < TEST_COUNT; j++) {
            arr[j] = rand() % TEST_COUNT;
            rbt.insert_unique(arr[j]);
        }
        rbt.disp();
        for (int j = 0; j < TEST_COUNT; j++)
            rbt.erase(arr[j]);
        assert(rbt.size() == 0);
    }
}

int main(int argc, const char* argv[]) {
    srand(time(NULL));
    std::vector<std::thread> threads;
    for (int i = 0; i < TEST_THREAD; i++) {
        threads.emplace_back(test_rbtree);
        std::this_thread::sleep_for(10ms);
    }
    for (auto&& t : threads)
        t.join();
    tiny_mempool::instance().report();
    return 0;
}