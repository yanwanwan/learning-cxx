#include "../exercise.h"

struct FibonacciCache {
    unsigned long long cache[16];
    int cached;
};

static unsigned long long fibonacci(FibonacciCache &cache, int i) {
    ASSERT(i >= 0 && i < 16, "i out of range");

    // 只要还没算到 i，就继续往后补
    for (; cache.cached <= i; ++cache.cached) {
        cache.cache[cache.cached] =
            cache.cache[cache.cached - 1] + cache.cache[cache.cached - 2];
    }
    return cache.cache[i];
}

int main(int argc, char **argv) {
    // 初始化：cache[0]=0, cache[1]=1, 从 2 开始补
    FibonacciCache fib{};        // 全部清零
    fib.cache[0] = 0;
    fib.cache[1] = 1;
    fib.cached = 2;

    ASSERT(fibonacci(fib, 10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fibonacci(fib, 10) << std::endl;
    return 0;
}
