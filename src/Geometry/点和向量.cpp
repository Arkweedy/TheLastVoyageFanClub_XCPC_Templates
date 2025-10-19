using T = long double;  // 全局数据类型

constexpr T eps = 1e-8;
constexpr T INF = numeric_limits<T>::max();
constexpr T PI = 3.1415926535897932384l;
struct Point {
    T x, y;

    bool operator==(const Point& a) const {
        return (abs(x - a.x) <= eps && abs(y - a.y) <= eps);
    }
    bool operator<(const Point& a) const {
        if (abs(x - a.x) <= eps) return y < a.y - eps;
        return x < a.x - eps;
    }
    bool operator>(const Point& a) const { return !(*this < a || *this == a); }
    Point operator+(const Point& a) const { return {x + a.x, y + a.y}; }
    Point operator-(const Point& a) const { return {x - a.x, y - a.y}; }
    Point operator-() const { return {-x, -y}; }
    Point operator*(const T k) const { return {k * x, k * y}; }
    Point operator/(const T k) const { return {x / k, y / k}; }
    T operator*(const Point& a) const { return x * a.x + y * a.y; }  // 点积
    T operator^(const Point& a) const {
        return x * a.y - y * a.x;
    }  // 叉积，注意优先级

    // 1: 左侧(逆时针) -1: 右侧(顺时针) 0: 共线
    int toleft(const Point& a) const {
        const auto t = (*this) ^ a;
        return (t > eps) - (t < -eps);
    }  // to-left 测试
    T len2() const { return (x * x + y * y); }  // 向量长度的平方
    T dis2(const Point& a) const {
        return (a - (*this)).len2();
    }  // 两点距离的平方
    int quad() const  // 象限判断 0:原点 1:x轴正 2:第一象限 3:y轴正 4:第二象限
                      // 5:x轴负 6:第三象限 7:y轴负 8:第四象限
    {
        if (abs(x) <= eps && abs(y) <= eps) return 0;
        if (abs(y) <= eps) return x > eps ? 1 : 5;
        if (abs(x) <= eps) return y > eps ? 3 : 7;
        return y > eps ? (x > eps ? 2 : 4) : (x > eps ? 8 : 6);
    }

    // 必须用浮点数
    T len() const { return sqrtl(x * x + y * y); }          // 向量长度
    T dis(const Point& a) const { return sqrtl(dis2(a)); }  // 两点距离
    T ang(const Point& a) const {
        return acosl(max(-1.0l, min(1.0l, ((*this) * a) / (len() * a.len()))));
    }  // 返回较小的向量夹角
    Point rot(const T rad) const {
        return {x * cos(rad) - y * sin(rad), x * sin(rad) + y * cos(rad)};
    }  // 逆时针旋转（给定角度）
    Point rot(const T cosr, const T sinr) const {
        return {x * cosr - y * sinr, x * sinr + y * cosr};
    }  // 逆时针旋转（给定角度的正弦与余弦）
};