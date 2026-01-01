#include "../exercise.h"
#include <cstring>

// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1;
        // TODO: 填入正确的 shape 并计算 size
        for (int i = 0; i < 4; ++i) {
            shape[i] = shape_[i];
            size *= shape[i];
        }
        data = new T[size];
        std::memcpy(data, data_, size * sizeof(T));
    }
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持“单向广播”。
    Tensor4D &operator+=(Tensor4D const &others) {
        // TODO: 实现单向广播的加法
        
        // 预先计算 others 的 stride (跨度)，用于将 4D 坐标转换为线性索引
        // index = n*stride0 + c*stride1 + h*stride2 + w*stride3
        // 这是一个优化，避免在内层循环重复计算乘法
        unsigned int o_stride[4];
        o_stride[3] = 1;
        o_stride[2] = others.shape[3];
        o_stride[1] = others.shape[2] * others.shape[3];
        o_stride[0] = others.shape[1] * others.shape[2] * others.shape[3];

        // 使用指针遍历 this->data，比每次计算 this 的线性索引更高效
        T* current_ptr = data;

        // 4层循环遍历 this 的每一个维度
        // 我们的目标是遍历 this 的每一个元素，找到它在 others 中对应的值加加上去
        for (unsigned int n = 0; n < shape[0]; ++n) {
            // 广播核心逻辑：如果 others 对应维度为 1，则索引固定为 0，否则随 n 变化
            unsigned int on = (others.shape[0] == 1) ? 0 : n;

            for (unsigned int c = 0; c < shape[1]; ++c) {
                unsigned int oc = (others.shape[1] == 1) ? 0 : c;

                for (unsigned int h = 0; h < shape[2]; ++h) {
                    unsigned int oh = (others.shape[2] == 1) ? 0 : h;

                    for (unsigned int w = 0; w < shape[3]; ++w) {
                        unsigned int ow = (others.shape[3] == 1) ? 0 : w;

                        // 计算 others 在其 data 数组中的线性索引
                        unsigned int other_idx = on * o_stride[0] + 
                                                 oc * o_stride[1] + 
                                                 oh * o_stride[2] + 
                                                 ow * o_stride[3];

                        // 执行加法
                        *current_ptr += others.data[other_idx];
                        
                        // 移动 this 的指针到下一个元素
                        ++current_ptr;
                    }
                }
            }
        }
        return *this;
    }
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}