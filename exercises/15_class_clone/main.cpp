#include "../exercise.h"

class DynFibonacci {
    size_t *cache;
    int cached;

public:
    // TODO: 实现动态设置容量的构造器
    DynFibonacci(int capacity): cache(new size_t[capacity]), cached(2) {
        cache[0] = 0;
        cache[1] = 1;
    }

    // TODO: 实现复制构造器
    // 1. 去掉 "= delete"
    // 2. 这里需要进行【深拷贝】：分配新的内存，然后把数据搬运过来
    DynFibonacci(DynFibonacci const &other) 
        : cache(new size_t[other.cached]), cached(other.cached) {
        // 将对方 cache 里的数据复制到自己的 cache 里
        for (int i = 0; i < cached; ++i) {
            cache[i] = other.cache[i];
        }
    }

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci() {
        delete[] cache;
    }

    // TODO: 实现正确的缓存优化斐波那契计算
    size_t get(int i) {
        for (; cached <= i; ++cached) {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
        }
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    size_t get(int i) const {
        if (i <= cached) {
            return cache[i];
        }
        ASSERT(false, "i out of range");
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    
    // 这里调用了复制构造函数
    DynFibonacci const fib_ = fib; 
    
    ASSERT(fib_.get(10) == fib.get(10), "Object cloned");
    return 0;
}