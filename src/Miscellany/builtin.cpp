#include <bits/stdc++.h>
using namespace std;

int main() {
    unsigned int a = 99;                            // 32位无符号整数
    unsigned long long b = 1234567890123456789ULL;  // 64位无符号长整型

    // --- 1. __builtin_clz / __builtin_clzll ---
    // 计算前导零的个数

    // 对于 a = 99 (二进制 ...01100011)
    // 32位表示: 00000000 00000000 00000000 01100011
    // 最高位的 '1' 在第7位（从右往左，0开始），所以前面有 32 - 7 = 25 个零。
    int clz_a = __builtin_clz(a);  // 25

    // 对于 b = 1234567890123456789ULL
    // 64位表示: 00010001 00101101 01001011 10000010 11010010 01010101 01100011
    // 00010101 最高位的 '1' 在第60位，所以前面有 64 - 61 = 3 个零。
    int clz_b = __builtin_clzll(b);  // 3
    // 对 0 调用 clz 是未定义行为

    // --- 2. __builtin_ctz / __builtin_ctzll ---
    // 计算末尾零的个数

    unsigned int c = 120;
    // 32位表示: 00000000 00000000 00000000 01111000
    // 最低位的 '1' 在第3位，所以后面有 3 个零。
    int ctz_c = __builtin_ctz(c);  // 3

    unsigned long long d = 96ULL;    // ...01100000
    int ctz_d = __builtin_ctzll(d);  // 5
    // 对 0 调用 ctz 是未定义行为

    // --- 3. __builtin_popcount / __builtin_popcountll ---
    // 计算二进制中 '1' 的个数

    // 对于 a = 99
    // 32位表示: 00000000 00000000 00000000 01100011, 共有4个'1'
    int popcount_a = __builtin_popcount(a);  // 4

    // 对于 b = 1234567890123456789ULL
    // 64位表示: 00010001 00101101 01001011 10000010 11010010 01010101 01100011
    // 00010101 共有32个'1'
    int popcount_b = __builtin_popcountll(b);  // 32

    // --- 4. __builtin_parity / __builtin_parityll ---
    // 检查 '1' 的个数是奇数还是偶数

    unsigned int f = 103;  // ...01100111, 有5个'1' (奇数)

    int parity_f = __builtin_parity(f);  // 1

    // b 有32个'1' (偶数)
    int parity_b = __builtin_parityll(b);  // 0

    return 0;
}
