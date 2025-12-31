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

    // TODO: 实现移动构造器
    // 这里的参数 "DynFibonacci &&" 表示右值引用
    DynFibonacci(DynFibonacci &&other) noexcept 
        : cache(other.cache), cached(other.cached) {
        // 关键步骤：把对方的指针置空
        // 这样对方析构时，delete nullptr 不会发生任何事
        other.cache = nullptr;
        other.cached = 0;
    }

    // TODO: 实现移动赋值
    // NOTICE: ⚠ 注意移动到自身问题 ⚠
    DynFibonacci &operator=(DynFibonacci &&other) noexcept {
        // 1. 检查自赋值：如果是自己移动给自己，直接返回
        if (this != &other) {
            // 2. 释放自己原有的资源 (防止内存泄漏)
            delete[] cache;
            
            // 3. 窃取对方的资源
            cache = other.cache;
            cached = other.cached;

            // 4. 将对方置空 (防止 Double Free)
            other.cache = nullptr;
            other.cached = 0;
        }
        return *this;
    }

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci() {
        // delete nullptr 是安全的 (no-op)，所以被移动后的对象可以安全析构
        delete[] cache;
    }

    // TODO: 实现正确的缓存优化斐波那契计算
    // 逻辑与之前的 get 方法完全一致，只是改了名字
    size_t operator[](int i) {
        for (; cached <= i; ++cached) {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
        }
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    size_t operator[](int i) const {
        ASSERT(i <= cached, "i out of range");
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    bool is_alive() const {
        return cache;
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib[10] == 55, "fibonacci(10) should be 55");

    // std::move(fib) 将 fib 转换为右值，触发移动构造函数
    DynFibonacci const fib_ = std::move(fib);
    // 此时 fib 的 cache 应该是 nullptr
    ASSERT(!fib.is_alive(), "Object moved");
    ASSERT(fib_[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci fib0(6);
    DynFibonacci fib1(12);

    // 触发移动赋值运算符
    fib0 = std::move(fib1);
    // 触发移动赋值的自赋值检查
    fib0 = std::move(fib0);
    ASSERT(fib0[10] == 55, "fibonacci(10) should be 55");

    return 0;
}