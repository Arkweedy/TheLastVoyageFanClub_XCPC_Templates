// p3 是一个三维点/向量类
// 并且需要实现了向量的基本运算: +, -, *, /, dot(), unit() (单位化) 等

using ld = long double;

// 定义一个用于浮点数比较的极小值 epsilon
const ld eps = 1e-9;

/**
 * @brief 判断浮点数 x 的符号
 * @param x 输入的浮点数
 * @return -1 (如果 x < 0), 0 (如果 x 在精度范围内为零), 1 (如果 x > 0)
 */
int sgn(ld x) {
    if (std::abs(x) < eps) return 0;
    return x < 0 ? -1 : 1;
}

// 三维点/向量结构体
struct p3 {
    ld x, y, z;

    // --- 基本运算符重载 ---
    p3 operator+(const p3 &a) const { return {x + a.x, y + a.y, z + a.z}; }
    p3 operator-(const p3 &a) const { return {x - a.x, y - a.y, z - a.z}; }
    p3 operator*(ld s) const { return {x * s, y * s, z * s}; }
    p3 operator/(ld s) const { return {x / s, y / s, z / s}; }

    // --- 比较运算符重载 ---
    // 基于 sgn 函数比较两个点/向量是否在精度范围内相等
    bool operator==(const p3 &a) const {
        return sgn(x - a.x) == 0 && sgn(y - a.y) == 0 && sgn(z - a.z) == 0;
    }

    // 计算向量的长度（模）
    ld len() const { return std::sqrtl(x * x + y * y + z * z); }

    // 将向量单位化（返回方向相同，长度为1的向量）
    p3 unit() const {
        ld l = len();
        // 处理零向量的情况，避免除以零
        if (sgn(l) == 0) return {0, 0, 0};
        return *this / l;
    }
};

/**
 * @brief 计算两个向量的点积 (Dot Product)
 */
ld dot(const p3 &a, const p3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
/**
 * @brief 计算旋转矩阵 A。
 * @param n 旋转轴（需要是单位向量）。
 * @param cosw 逆时针旋转角度的余弦值。
 * 一个点 (x_old, y_old, z_old) 变换后的坐标为
 * new[i] = sum(old[j] * A[i][j]) for j=0..2
 */
void calc(p3 n, ld cosw) {
    ld sinw = sqrt(1 - cosw * cosw);  // 计算旋转角度的正弦值
    n = n.unit();                     // 将旋转轴向量单位化
    for (int i = 0; i < 3; i++) {
        int j = (i + 1) % 3, k = (j + 1) % 3;
        ld x = n[i], y = n[j], z = n[k];
        // 对角线元素
        A[i][i] = (y * y + z * z) * cosw + x * x;
        // 非对角线元素
        A[i][j] = x * y * (1 - cosw) + z * sinw;
        A[i][k] = x * z * (1 - cosw) - y * sinw;
    }
}

// 计算两个三维向量的叉积
p3 cross(p3 a, p3 b) {
    return p3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
              a.x * b.y - a.y * b.x);
}

/**
 * @brief 计算三个三维向量的混合积
 * @return 混合积 dot(cross(a, b), c)。其绝对值等于由 a, b, c
 * 构成的平行六面体的体积。
 */
ld mix(p3 a, p3 b, p3 c) { return dot(cross(a, b), c); }

// 三维直线/线段结构体
struct l3 {
    p3 s, t;  // 由两点 s, t 定义，方向向量为 t - s
};

// 三维平面结构体
struct plane {
    p3 nor;  // 平面的单位法向量
    ld m;    // 原点到平面的有向距离 (满足 dot(nor, P) = m, 其中 P
             // 是平面上任意一点)

    /**
     * @brief 构造函数：通过一个法向量 r 和平面上一点 a 来定义平面。
     */
    plane(p3 r, p3 a) : nor(r.unit()), m(dot(nor, a)) {}
};

/**
 * @brief 计算点 a 在平面 b 上的投影点。
 * @param a 空间中的一点。
 * @param b 目标平面。
 * @return 点 a 在平面 b 上的正交投影点。
 */
p3 project_to_plane(p3 a, plane b) {
    // a 到平面的有向距离为 (dot(a, b.nor) - b.m)
    // 从 a 点沿着法向量反方向移动这段距离即可到达投影点
    return a + b.nor * (b.m - dot(a, b.nor));
}

/**
 * @brief 计算两条三维直线 x 和 y 之间的最近点对。
 * @param x 第一条直线。
 * @param y 第二条直线。
 * @return 一个 pair，包含在直线 x 和 y 上的最近点。
 * @note 通过求解线性方程组找到使两点间距离最小化的参数 s 和 t。
 * P(s) = x.s + (x.t - x.s) * s
 * Q(t) = y.s + (y.t - y.s) * t
 */
pair<p3, p3> l3_closest(l3 x, l3 y) {
    p3 dx = x.t - x.s;
    p3 dy = y.t - y.s;
    ld a = dot(dx, dx);
    ld b = dot(dx, dy);
    ld e = dot(dy, dy);
    ld d = a * e - b * b;  // 行列式，如果为0则两直线平行

    p3 r = x.s - y.s;
    ld c = dot(dx, r);
    ld f = dot(dy, r);

    // 求解参数 s 和 t
    ld s = (b * f - c * e) / d;
    ld t = (a * f - c * b) / d;

    return {x.s + dx * s, y.s + dy * t};
}

/**
 * @brief 计算直线 b 与平面 a 的交点。
 * @param a 平面。
 * @param b 直线。
 * @return 交点坐标。
 * @warning 需要注意分母为零的情况（直线与平面平行）。
 */
p3 intersect(plane a, l3 b) {
    // 将直线方程 P(t) = b.s + (b.t - b.s) * t 代入平面方程 a.nor · P = a.m
    // 解出参数 t
    ld t = dot(a.nor, a.nor * a.m - b.s) / dot(a.nor, b.t - b.s);
    return b.s + (b.t - b.s) * t;
}

/**
 * @brief 计算两个平面 a 和 b 的交线。
 * @param a 第一个平面。
 * @param b 第二个平面。
 * @return 表示交线的 l3 结构体。
 * @warning 需要注意两平面平行的情况 (叉积为零向量)。
 */
l3 intersect(plane a, plane b) {
    // 交线的方向向量垂直于两个平面的法向量
    p3 d = cross(a.nor, b.nor);
    // 为了找到交线上的一个点，可以构造第三个平面 (法向量为 d) 再求三平面交点
    // 这里使用了一种更直接的几何解法
    p3 d2 = cross(b.nor, d);
    ld t = dot(d2, a.nor);
    // s 是交线上的一个点
    p3 s = d2 * (a.m - dot(b.nor * b.m, a.nor)) / t + b.nor * b.m;
    return {s, s + d};
}

/**
 * @brief 计算三个平面 a, b, c 的交点。
 * @param a 第一个平面。
 * @param b 第二个平面。
 * @param c 第三个平面。
 * @return 三个平面的唯一交点。
 * @warning 需要注意分母 mix(c1,c2,c3) 为零的情况（三个平面不交于一点）。
 */
p3 intersect(plane a, plane b, plane c) {
    // 方法一：先求出 b, c 的交线，再求该直线与平面 a 的交点
    // return intersect(a, intersect(b, c));

    // 方法二：使用克莱姆法则求解由三个平面方程构成的线性方程组
    p3 c1(a.nor.x, b.nor.x, c.nor.x);  // 系数矩阵的列向量
    p3 c2(a.nor.y, b.nor.y, c.nor.y);
    p3 c3(a.nor.z, b.nor.z, c.nor.z);
    p3 c4(a.m, b.m, c.m);  // 常数项向量

    // mix(c1, c2, c3) 是系数矩阵的行列式
    return 1 / mix(c1, c2, c3) *
           p3(mix(c4, c2, c3), mix(c1, c4, c3), mix(c1, c2, c4));
}
