#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int t = 0;
    while (1) {
        cout << "test: " << t++ << endl;
        system("data.exe > data.in");
        system("std.exe < data.in > std.out");
        system("solve.exe < data.in > solve.out");
        if (system("fc std.out solve.out > diff.log")) {
            cout << "WA\n";
            break;
        }
        cout << "AC\n";
    }
    return 0;
}

// Linux
#include <cstdlib>  // 为了 system() 函数
#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);

    for (int t = 1;; ++t) {
        cout << "Test #" << t << ": ";

        // 1. 运行数据生成器 (注意：没有 .exe 后缀，且需要用 ./ 来执行)
        system("./data > data.in");

        // 2. 运行标程 (std) 和你的解法 (solve)
        system("./std < data.in > std.out");
        system("./solve < data.in > solve.out");

        // 3. 使用 diff 命令比较输出文件
        //    "diff -q"
        //    表示安静模式(quiet)，只通过退出码告知结果，不输出差异详情
        //    "> /dev/null" 将 diff 的标准输出重定向到“黑洞”，确保屏幕干净
        if (system("diff -q -Z std.out solve.out > /dev/null")) {
            cout << "WA (Wrong Answer)!" << endl;
            cout << "Input data is in 'data.in'" << endl;
            cout << "Standard output is in 'std.out'" << endl;
            cout << "Your output is in 'solve.out'" << endl;
            break;  // 发现错误，停止对拍
        }

        cout << "AC (Accepted)" << endl;
    }

    return 0;
}