struct Line {
    Point p, v;  // p 为直线上一点，v 为方向向量

    bool operator==(const Line &a) const {
        return v.toleft(a.v) == 0 && v.toleft(p - a.p) == 0;
    }
    int toleft(const Point &a) const {
        return v.toleft(a - p);
    }  // to-left 测试
    bool operator<(const Line &a) const  // 半平面交算法定义的排序
    {
        if (abs(v ^ a.v) <= eps && v * a.v >= -eps) return toleft(a.p) == -1;
        return Argcmp()(v, a.v);
    }

    // 必须用浮点数
    Point inter(const Line &a) const {
        return p + v * ((a.v ^ (p - a.p)) / (v ^ a.v));
    }  // 直线交点
    ld dis(const Point &a) const {
        return abs(v ^ (a - p)) / v.len();
    }  // 点到直线距离
    Point proj(const Point &a) const {
        return p + v * ((v * (a - p)) / (v * v));
    }  // 点在直线上的投影
};