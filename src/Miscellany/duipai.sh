#!/bin/bash

# 编译所有程序
echo "Compiling..."
g++ -std=c++20 -O2 data.cpp -o data
g++ -std=c++20 -O2 std.cpp -o std
g++ -std=c++20 -O2 solve.cpp -o solve
echo "Compilation finished."

# 无限循环进行测试
i=1
while true; do
    echo -n "Test #$i: " # -n 表示不换行

    # 生成数据并运行程序
    ./data > data.in
    ./std < data.in > std.out
    ./solve < data.in > solve.out

    # 比较文件
    if diff -q -Z std.out solve.out > /dev/null; then
        echo "AC"
    else
        echo "WA!"
        echo "Input, std output, and your output are in data.in, std.out, solve.out"
        break # 退出循环
    fi
    ((i++)) # 计数器加一
done