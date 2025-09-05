#include <bits/stdc++.h>
using namespace std;

const double eps = 1e-8;
const double PI = acos(-1);

struct Point {
    double x, y;

    Point() = default;
    Point(double x, double y) : x(x), y(y) {}

    bool operator==(const Point &a) const {
        return (abs(x - a.x) <= eps && abs(y - a.y) <= eps);
    }
    Point operator+(const Point &a) const { return Point(x + a.x, y + a.y); }
    Point operator-(const Point &a) const { return Point(x - a.x, y - a.y); }
    Point operator-() const { return Point(-x, -y); }
    Point operator*(const double &k) const { return Point(k * x, k * y); }
    Point operator/(const double &k) const { return Point(x / k, y / k); }
    double operator*(const Point &a) const { return x * a.x + y * a.y; }  // Dot
    double operator^(const Point &a) const {
        return x * a.y - y * a.x;
    }  // Cross
    bool operator<(const Point &a) const {
        if (abs(x - a.x) <= eps) return y < a.y - eps;
        return x < a.x - eps;
    }
    bool is_par(const Point &a) const { return abs((*this) ^ a) <= eps; }
    bool is_ver(const Point &a) const { return abs((*this) * a) <= eps; }
    double len() const { return sqrt((*this) * (*this)); }
    double dis(const Point &a) const { return (a - (*this)).len(); }
    double ang(const Point &a) const {
        return acos(((*this) * a) / (this->len() * a.len()));
    }
    Point rot(const double &rad) const {
        return Point(x * cos(rad) - y * sin(rad), x * sin(rad) + y * cos(rad));
    }
};

bool argcmp(const Point &a, const Point &b) {
    int ha = 0, hb = 0;
    if (a.y < -eps || (abs(a.y) <= eps && a.x >= -eps))
        ha = -1;
    else
        ha = 1;
    if (b.y < -eps || (abs(b.y) <= eps && b.x >= -eps))
        hb = -1;
    else
        hb = 1;
    if (ha != hb) return ha < hb;
    auto t = (a == Point(0, 0) ? Point(1, 0) : a) ^
             (b == Point(0, 0) ? Point(1, 0) : b);
    if (abs(t) <= eps) return a * a < b * b - eps;
    return t > eps;
}

struct Convex {
    set<Point, decltype(&argcmp)> p{&argcmp};  // 坐标扩大三倍，便于整数运算
    Point o;                                   // 凸包内一点

    inline auto nxt(decltype(p.begin()) it) const {
        it++;
        return it == p.end() ? p.begin() : it;
    }
    inline auto pre(decltype(p.begin()) it) const {
        if (it == p.begin()) it = p.end();
        return --it;
    }

    bool is_in(const Point &a) const {
        if (p.size() <= 1) return false;
        auto it = p.lower_bound(a * 3 - o);
        if (it == p.end()) it = p.begin();
        return ((*it - *pre(it)) ^ ((a * 3 - o) - *pre(it))) >= -eps;
    }

    void insert(Point a) {
        if (p.size() <= 1) {
            p.insert(a * 3);
            return;
        }
        if (p.size() == 2) {
            Point u = *p.begin(), v = *p.rbegin();
            o = (u + v + a * 3) / 3;
            p.clear();
            p.insert(u - o);
            p.insert(v - o);
            p.insert(a * 3 - o);
            return;
        }
        if (is_in(a)) return;
        a = a * 3 - o;
        auto _it = p.insert(a).first;
        auto it = nxt(_it);
        while (p.size() > 3 && ((*it - a) ^ (*nxt(it) - *it)) <= eps)
            p.erase(it), it = nxt(_it);
        it = pre(_it);
        while (p.size() > 3 && ((*it - *pre(it)) ^ (a - *it)) <= eps)
            p.erase(it), it = pre(_it);
    }
};

int read_int() {
    int re = 0;
    char c = 0;
    bool sgn = false;
    while (!isdigit(c) && c != '-') c = getchar();
    if (c == '-') sgn = true, c = getchar();
    while (isdigit(c)) {
        re = re * 10 + c - '0';
        c = getchar();
    }
    return sgn ? -re : re;
}

int q, o;
long long x, y;

int main() {
    Convex c;
    scanf("%d", &q);
    while (q--) {
        scanf("%d%lld%lld", &o, &x, &y);
        if (o == 1)
            c.insert(Point(x, y));
        else if (c.is_in(Point(x, y)))
            printf("YES\n");
        else
            printf("NO\n");
    }
    getchar();
    getchar();
    return 0;
}