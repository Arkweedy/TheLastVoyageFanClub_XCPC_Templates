struct Bigint {
    // a 用于存储数字，为了方便计算，数字是倒序存储的。
    // 例如，数字 123 在字符串 a 中存储为 "321"。
    string a;
    // sign 表示符号，1 代表正数或零，-1 代表负数。
    int sign;

    // 默认构造函数
    Bigint() : sign(1) {}

    // 从字符串构造 Bigint
    Bigint(string b) { (*this) = b; }

    // 获取数字的位数
    int size() { return a.size(); }

    // 翻转符号（正变负，负变正）
    Bigint inverseSign() {
        sign *= -1;
        return (*this);
    }

    // 规范化：移除前导零并设置正确的符号
    Bigint normalize(int newSign) {
        sign = newSign;
        // 从字符串末尾（即数字的最高位）移除前导零
        for (int i = a.size() - 1; i > 0 && a[i] == '0'; i--) {
            a.erase(a.begin() + i);
        }
        // 如果结果是 "0"，符号应为正
        if (a.size() == 1 && a[0] == '0') {
            sign = 1;
        }
        return (*this);
    }

    // 赋值运算符：从字符串赋值
    void operator=(string b) {
        // 如果字符串以'-'开头，则a存储'-'之后的部分，否则存储整个字符串
        a = b[0] == '-' ? b.substr(1) : b;
        // 将数字倒序存储
        reverse(a.begin(), a.end());
        // 规范化并设置符号
        this->normalize(b[0] == '-' ? -1 : 1);
    }

    // 小于运算符
    bool operator<(const Bigint& b) const {
        if (sign != b.sign) return sign < b.sign;  // 符号不同，负数小
        if (a.size() != b.a.size()) {
            // 符号相同，位数不同
            // 正数：位数少的数字小
            // 负数：位数多的数字小 (因为其绝对值更大)
            return sign == 1 ? a.size() < b.a.size() : a.size() > b.a.size();
        }
        // 符号和位数都相同，从高位到低位逐位比较
        for (int i = a.size() - 1; i >= 0; i--) {
            if (a[i] != b.a[i]) {
                // 正数：字符小的数值小
                // 负数：字符大的数值小 (因为其绝对值更大)
                return sign == 1 ? a[i] < b.a[i] : a[i] > b.a[i];
            }
        }
        return false;  // 两数相等
    }

    // 等于运算符
    bool operator==(const Bigint& b) const {
        return a == b.a && sign == b.sign;
    }

    // 加法运算符
    Bigint operator+(Bigint b) {
        // 如果符号不同，转化为减法
        if (sign != b.sign) return (*this) - b.inverseSign();
        Bigint c;
        c.a = "";
        for (int i = 0, carry = 0; i < a.size() || i < b.size() || carry; i++) {
            carry += (i < a.size() ? a[i] - '0' : 0) +
                     (i < b.size() ? b.a[i] - '0' : 0);
            c.a += (carry % 10 + '0');
            carry /= 10;
        }
        return c.normalize(sign);
    }

    // 减法运算符
    Bigint operator-(Bigint b) {
        // 如果符号不同，转化为加法
        if (sign != b.sign) return (*this) + b.inverseSign();
        // 保证大数减小数
        if ((*this) < b) return (b - (*this)).inverseSign();
        Bigint c;
        c.a = "";
        for (int i = 0, borrow = 0; i < a.size(); i++) {
            borrow = a[i] - borrow - (i < b.size() ? b.a[i] : '0');
            c.a += borrow >= 0 ? borrow + '0' : borrow + 10 + '0';
            borrow = borrow >= 0 ? 0 : 1;
        }
        return c.normalize(sign);
    }

    // 乘法运算符 (朴素的 O(N^2) 实现)
    // 逻辑：模拟手算，将 b 累加 a[i] * 10^i 次
    Bigint operator*(Bigint b) {
        Bigint c("0");
        for (int i = 0, k = 0; i < a.size(); i++) {
            k = a[i] - '0';
            Bigint temp = b;  // 临时变量存储当前位的乘积
            while (--k > 0) temp = temp + b;
            if (a[i] != '0') c = c + temp;
            b.a.insert(b.a.begin(), '0');  // 相当于 b *= 10，为下一位做准备
        }
        return c.normalize(sign * b.sign);
    }

    // 除法运算符 (朴素的 O(N^2) 实现)
    Bigint operator/(Bigint b) {
        // 除数不能为0
        if (b.size() == 1 && b.a[0] == '0') {
            b.a[0] /= (b.a[0] - '0');  // 触发一个除以零的异常
        }
        Bigint c("0"), d;
        d.a = "";
        for (int j = 0; j < a.size(); j++) d.a += "0";  // 初始化商
        int dSign = sign * b.sign;
        b.sign = 1;  // 除数取绝对值进行计算

        // 从高位开始逐位试商
        for (int i = a.size() - 1; i >= 0; i--) {
            c.a.insert(c.a.begin(), '0');  // 余数左移一位
            c = c + a.substr(i, 1);        // 加入当前位
            c.normalize(1);
            while (!(c < b)) {
                c = c - b;
                d.a[i]++;  // 商的当前位增加
            }
        }
        return d.normalize(dSign);
    }

    // 取模运算符
    Bigint operator%(Bigint b) {
        // 除数不能为0
        if (b.size() == 1 && b.a[0] == '0') {
            b.a[0] /= (b.a[0] - '0');
        }
        Bigint c("0");
        b.sign = 1;  // 除数取绝对值

        // 计算过程与除法类似，最后得到的 c 就是余数
        for (int i = a.size() - 1; i >= 0; i--) {
            c.a.insert(c.a.begin(), '0');
            c = c + a.substr(i, 1);
            c.normalize(1);
            while (!(c < b)) {
                c = c - b;
            }
        }
        // C++中，a % b 的结果符号与 a 相同
        return c.normalize(sign);
    }

    // 输出函数
    void print() {
        if (sign == -1) cout << "-";
        for (int i = a.size() - 1; i >= 0; i--) {
            cout << a[i];
        }
    }
};