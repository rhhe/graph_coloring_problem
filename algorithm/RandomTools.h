#ifndef GRAPH_COLORING_PROBLEM_RANDOMTOOLS_H
#define GRAPH_COLORING_PROBLEM_RANDOMTOOLS_H

#include <random>

class RandomTools {
public:
    RandomTools() { InitializeInner(); };

    explicit RandomTools(unsigned seed) : seed_(seed) { InitializeInner(); }

    virtual ~RandomTools() = default;

    unsigned seed_ = 12111;
    std::mt19937 *mt_{};
    std::uniform_real_distribution<double> distribution_{0.0, 1.0};

    /**
     * 随机选择一个数
     * @param 数组大小
     * @return 被选中的数
     */
    int SelectUniformRandomly(int size) {
        return static_cast<int>(distribution_(*mt_) * size) % size;
    }

protected:
    void InitializeInner() {
        mt_ = new std::mt19937(randomDevice_());
        mt_->seed(seed_);
    }

    std::random_device randomDevice_;
};


#endif //GRAPH_COLORING_PROBLEM_RANDOMTOOLS_H
