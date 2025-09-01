struct Circle {
    Point c;
    T r;  // 一般来说必须用浮点数

    bool operator==(const Circle &a) const {
        return c == a.c && abs(r - a.r) <= eps;
    }
    T circ() const { return 2 * PI * r; }  // 周长
    T area() const { return PI * r * r; }  // 面积

    // 点与圆的关系
    // -1 圆上 | 0 圆外 | 1 圆内
    int is_in(const Point &p) const {
        const T d = p.dis(c);
        return abs(d - r) <= eps ? -1 : d < r - eps;
    }

    // 直线与圆关系
    // 0 相离 | 1 相切 | 2 相交
    int relation(const Line &l) const {
        const T d = l.dis(c);
        if (d > r + eps) return 0;
        if (abs(d - r) <= eps) return 1;
        return 2;
    }

    // 圆与圆关系
    // -1 相同 | 0 相离 | 1 外切 | 2 相交 | 3 内切 | 4 内含
    int relation(const Circle &a) const {
        if (*this == a) return -1;
        const T d = c.dis(a.c);
        if (d > r + a.r + eps) return 0;
        if (abs(d - r - a.r) <= eps) return 1;
        if (abs(d - abs(r - a.r)) <= eps) return 3;
        if (d < abs(r - a.r) - eps) return 4;
        return 2;
    }

    // 直线与圆的交点
    vector<Point> inter(const Line &l) const {
        const T d = l.dis(c);
        const Point p = l.proj(c);
        const int t = relation(l);
        if (t == 0) return vector<Point>();
        if (t == 1) return vector<Point>{p};
        const T k = sqrt(r * r - d * d);
        return vector<Point>{p - (l.v / l.v.len()) * k,
                             p + (l.v / l.v.len()) * k};
    }

    // 圆与圆交点
    vector<Point> inter(const Circle &a) const {
        const T d = c.dis(a.c);
        const int t = relation(a);
        if (t == -1 || t == 0 || t == 4) return vector<Point>();
        Point e = a.c - c;
        e = e / e.len() * r;
        if (t == 1 || t == 3) {
            if (r * r + d * d - a.r * a.r >= -eps) return vector<Point>{c + e};
            return vector<Point>{c - e};
        }
        const T costh = (r * r + d * d - a.r * a.r) / (2 * r * d),
                sinth = sqrt(1 - costh * costh);
        return vector<Point>{c + e.rot(costh, -sinth), c + e.rot(costh, sinth)};
    }

    // 圆与圆交面积
    T inter_area(const Circle &a) const {
        const T d = c.dis(a.c);
        const int t = relation(a);
        if (t == -1) return area();
        if (t < 2) return 0;
        if (t > 2) return min(area(), a.area());
        const T costh1 = (r * r + d * d - a.r * a.r) / (2 * r * d),
                costh2 = (a.r * a.r + d * d - r * r) / (2 * a.r * d);
        const T sinth1 = sqrt(1 - costh1 * costh1),
                sinth2 = sqrt(1 - costh2 * costh2);
        const T th1 = acos(costh1), th2 = acos(costh2);
        return r * r * (th1 - costh1 * sinth1) +
               a.r * a.r * (th2 - costh2 * sinth2);
    }

    // 过圆外一点圆的切线
    vector<Line> tangent(const Point &a) const {
        const int t = is_in(a);
        if (t == 1) return vector<Line>();
        if (t == -1) {
            const Point v = {-(a - c).y, (a - c).x};
            return vector<Line>{{a, v}};
        }
        Point e = a - c;
        e = e / e.len() * r;
        const T costh = r / c.dis(a), sinth = sqrt(1 - costh * costh);
        const Point t1 = c + e.rot(costh, -sinth), t2 = c + e.rot(costh, sinth);
        return vector<Line>{{a, t1 - a}, {a, t2 - a}};
    }

    // 两圆的公切线
    vector<Line> tangent(const Circle &a) const {
        const int t = relation(a);
        vector<Line> lines;
        if (t == -1 || t == 4) return lines;
        if (t == 1 || t == 3) {
            const Point p = inter(a)[0], v = {-(a.c - c).y, (a.c - c).x};
            lines.push_back({p, v});
        }
        const T d = c.dis(a.c);
        const Point e = (a.c - c) / (a.c - c).len();
        if (t <= 2) {
            const T costh = (r - a.r) / d, sinth = sqrt(1 - costh * costh);
            const Point d1 = e.rot(costh, -sinth), d2 = e.rot(costh, sinth);
            const Point u1 = c + d1 * r, u2 = c + d2 * r, v1 = a.c + d1 * a.r,
                        v2 = a.c + d2 * a.r;
            lines.push_back({u1, v1 - u1});
            lines.push_back({u2, v2 - u2});
        }
        if (t == 0) {
            const T costh = (r + a.r) / d, sinth = sqrt(1 - costh * costh);
            const Point d1 = e.rot(costh, -sinth), d2 = e.rot(costh, sinth);
            const Point u1 = c + d1 * r, u2 = c + d2 * r, v1 = a.c - d1 * a.r,
                        v2 = a.c - d2 * a.r;
            lines.push_back({u1, v1 - u1});
            lines.push_back({u2, v2 - u2});
        }
        return lines;
    }

    // 圆的反演
    // auto result = circle.inverse(line);
    // if (std::holds_alternative<Circle>(result))
    // Circle c = std::get<Circle>(result);
    std::variant<Circle, Line> inverse(const Line &l) const {
        if (l.toleft(c) == 0) return l;
        const Point v =
            l.toleft(c) == 1 ? Point{l.v.y, -l.v.x} : Point{-l.v.y, l.v.x};
        const T d = r * r / l.dis(c);
        const Point p = c + v / v.len() * d;
        return Circle{(c + p) / 2, d / 2};
    }

    std::variant<Circle, Line> inverse(const Circle &a) const {
        const Point v = a.c - c;
        if (a.is_in(c) == -1) {
            const T d = r * r / (a.r + a.r);
            const Point p = c + v / v.len() * d;
            return Line{p, {-v.y, v.x}};
        }
        if (c == a.c) return Circle{c, r * r / a.r};
        const T d1 = r * r / (c.dis(a.c) - a.r),
                d2 = r * r / (c.dis(a.c) + a.r);
        const Point p = c + v / v.len() * d1, q = c + v / v.len() * d2;
        return Circle{(p + q) / 2, p.dis(q) / 2};
    }
};