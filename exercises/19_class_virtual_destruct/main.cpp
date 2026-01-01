#include "../exercise.h"

struct A {
    // 1. 类内只做声明
    static int num_a;

    A() {
        ++num_a;
    }
    
    // 4. CRITICAL: 基类析构函数必须是 virtual
    // 否则 delete ab (A类型指针) 时，不会调用 B 的析构函数
    virtual ~A() {
        --num_a;
    }

    virtual char name() const {
        return 'A';
    }
};

// 2. 类外定义并初始化静态变量
int A::num_a = 0;

struct B final : public A {
    static int num_b;

    B() {
        ++num_b;
    }
    ~B() {
        --num_b;
    }

    char name() const final {
        return 'B';
    }
};

int B::num_b = 0;

int main(int argc, char **argv) {
    auto a = new A;
    auto b = new B;
    
    // A::num_a = 1(from a) + 1(from b) = 2
    ASSERT(A::num_a == 2, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(a->name() == 'A', "Fill in the correct value for a->name()");
    ASSERT(b->name() == 'B', "Fill in the correct value for b->name()");

    delete a;
    delete b;
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    A *ab = new B; // 派生类指针可以随意转换为基类指针
    ASSERT(A::num_a == 1, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(ab->name() == 'B', "Fill in the correct value for ab->name()");

    // TODO: 基类指针无法随意转换为派生类指针，补全正确的转换语句
    // 使用 dynamic_cast 进行安全的向下转型 (需要多态基类)
    // 或者 static_cast (如果我们确信它就是 B)
    B &bb = dynamic_cast<B&>(*ab); 
    ASSERT(bb.name() == 'B', "Fill in the correct value for bb->name()");

    // TODO: ---- 以下代码不要修改，通过改正类定义解决编译问题 ----
    
    // 如果 A 的析构函数不是 virtual，这里只会调用 ~A()，跳过 ~B()
    // 导致 num_b 还是 1，断言失败。
    delete ab; 
    
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    return 0;
}