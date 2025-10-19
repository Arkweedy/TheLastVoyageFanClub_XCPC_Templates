#include <chrono>  // 关键头文件
#include <iostream>
#include <thread>  // 用于 std::this_thread::sleep_for 来模拟耗时操作

// 模拟一个耗时的计算函数
void heavy_computation() {
    long long sum = 0;
    for (int i = 0; i < 200000000; ++i) {
        sum += i;
    }
    // 使用 volatile 防止编译器优化掉整个循环
    volatile long long final_sum = sum;
}

int main() {
    // 1. 在要计时的代码段开始前，获取当前时间点
    auto start_time = std::chrono::high_resolution_clock::now();

    // 2. 执行你想要测量的代码
    heavy_computation();

    // 3. 在代码段结束后，再次获取当前时间点
    auto end_time = std::chrono::high_resolution_clock::now();

    // 4. 计算两个时间点之间的差值
    auto duration = end_time - start_time;

    // 5. 将时间差转换为你想要的单位（纳秒、微秒、毫秒、秒等）
    // duration_cast 是一个安全的类型转换工具
    auto duration_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    auto duration_s =
        std::chrono::duration_cast<std::chrono::duration<double>>(duration);

    // 6. 输出结果
    // .count() 方法返回时间单位的数量
    std::cout << "\n--- Timing Results ---" << std::endl;
    std::cout << "Execution time: " << duration_ms.count() << " milliseconds"
              << std::endl;
    std::cout << "Execution time: " << duration_s.count() << " seconds"
              << std::endl;

    return 0;
}