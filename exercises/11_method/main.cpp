#include "../exercise.h"
#include <iostream>

struct Fibonacci {
    unsigned long long cache[128];
    int cached;

    // TODO: 实现正确的缓存优化斐波那契计算
    unsigned long long get(int i) {
        ASSERT(i >= 0 && i < 128, "i out of range");
        
        // 1. 如果请求的 i 已经在缓存里 (i < cached)，直接返回，不用算
        if (i < cached) {
            return cache[i];
        }

        // 2. 如果不在缓存里，从当前 cached 开始计算，一直算到 i 为止
        for (; cached <= i; ++cached) {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
        }

        return cache[i];
    }
};

int main(int argc, char **argv) {
    // TODO: 初始化缓存结构体，使计算正确
    Fibonacci fib{};
    // 斐波那契数列前两项是固定的
    fib.cache[0] = 0;
    fib.cache[1] = 1;
    // 我们已经有了 0 和 1，所以下一次应该从 index 2 开始计算
    fib.cached = 2; 

    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    return 0;
}