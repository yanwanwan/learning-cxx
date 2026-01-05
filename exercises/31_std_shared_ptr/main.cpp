#include "../exercise.h"
#include <memory>

// READ: `std::shared_ptr` <https://zh.cppreference.com/w/cpp/memory/shared_ptr>
// READ: `std::weak_ptr` <https://zh.cppreference.com/w/cpp/memory/weak_ptr>

// TODO: 将下列 `?` 替换为正确的值
int main(int argc, char **argv) {
    auto shared = std::make_shared<int>(10);
    std::shared_ptr<int> ptrs[]{shared, shared, shared};

    std::weak_ptr<int> observer = shared;
    ASSERT(observer.use_count() == 4, ""); // shared + ptrs[0~2]

    ptrs[0].reset();
    ASSERT(observer.use_count() == 3, ""); // shared + ptrs[1~2]

    ptrs[1] = nullptr;
    ASSERT(observer.use_count() == 2, ""); // shared + ptrs[2]

    // 关键点：make_shared 创建了一个*新*对象，ptrs[2] 指向新对象，放弃了老对象
    ptrs[2] = std::make_shared<int>(*shared);
    ASSERT(observer.use_count() == 1, ""); // 仅剩 shared

    ptrs[0] = shared;
    ptrs[1] = shared;
    // move 后，shared 变空，所有权转移给 ptrs[2]
    ptrs[2] = std::move(shared);
    ASSERT(observer.use_count() == 3, ""); // ptrs[0], ptrs[1], ptrs[2]

    std::ignore = std::move(ptrs[0]);
    ptrs[1] = std::move(ptrs[1]);
    ptrs[1] = std::move(ptrs[2]);
    ASSERT(observer.use_count() == 2, ""); // <--- 改成 2

    shared = observer.lock();
    ASSERT(observer.use_count() == 3, ""); // <--- 改成 3

    shared = nullptr;
    for (auto &ptr : ptrs) ptr = nullptr;
    ASSERT(observer.use_count() == 0, "");
    shared = observer.lock();
    ASSERT(observer.use_count() == 0, "");
    
    // 对象已销毁，lock 失败
    shared = observer.lock();
    ASSERT(observer.use_count() == 0, "");

    return 0;
}