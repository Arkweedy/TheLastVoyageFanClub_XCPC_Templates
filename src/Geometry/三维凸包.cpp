


// ------------------------------------------------------------------
// 三维凸包
// 使用说明:
// 1. 将所有三维点放入全局变量 vector<p3> p 中。
// 2. 调用 solve() 函数。
// 3. 如果 solve() 返回 true, 则凸包构造成功。
// 4. 结果存储在全局变量 vector<Face> face 中,
//    其中每个 Face 是一个包含三个整数的数组 {a, b, c},
//    代表凸包的一个三角面, 由点 p[a], p[b], p[c] 构成。
//
// 前置依赖:
// - p3: 三维点/向量类。
// - mix(): 三个向量的混合积。
// - cross(): 两个向量的叉积。
// - N: 一个略大于预计处理的最大点数的常量, 用于 mark 数组。
// ------------------------------------------------------------------

const int N = 2010;

vector<p3> p;                // 全局变量, 存储所有输入的点
int mark[N][N], stp;         // 标记数组, 用于处理边界
typedef array<int, 3> Face;  // Face 类型, 用三个点的索引表示一个三角面
vector<Face> face;           // 全局变量, 存储最终凸包的所有面

/**
 * @brief 计算四点构成的四面体的有向体积。
 * @return 体积 > 0 表示点 d 在平面(a,b,c)的法向量正方向一侧。
 */
LD volume(int a, int b, int c, int d) {
    return mix(p[b] - p[a], p[c] - p[a], p[d] - p[a]);
}

/**
 * @brief (内部函数) 向凸包面列表中添加一个新面。
 */
void ins(int a, int b, int c) { face.push_back({a, b, c}); }

/**
 * @brief (内部函数) 将点 v 加入到当前的凸包中。
 * 算法会找到所有 v 能 "看见" 的面, 删除它们,
 * 然后用 v 和"视线"边界上的边构成新面。
 */
void add(int v) {
    vector<Face> tmp;
    stp++;  // 更新时间戳
    // 找出所有对点 v 可见的面 (volume < 0), 并标记其边
    for (auto f : face) {
        if (sgn(volume(v, f[0], f[1], f[2])) < 0) {
            for (auto i : f)
                for (auto j : f) mark[i][j] = stp;
        } else {
            tmp.push_back(f);
        }
    }
    face = tmp;  // 更新面列表, 只保留 v 看不见的面
    // 遍历留下来的面的边, 如果该边是可见与不可见的分界线, 则与 v 构成新面
    for (int i = 0; i < (int)tmp.size(); i++) {
        int a = tmp[i][0], b = tmp[i][1], c = tmp[i][2];
        if (mark[a][b] == stp) ins(b, a, v);
        if (mark[b][c] == stp) ins(c, b, v);
        if (mark[c][a] == stp) ins(a, c, v);
    }
}

const ld eps = 1e-9; 

/**
 * @brief 判断浮点数 x 的符号
 * @param x 输入的浮点数
 * @return -1 表示 x 为负数
 * 0 表示 x 在精度范围内可视为零
 * 1 表示 x 为正数
 */
int sgn(ld x) {
    if (std::abs(x) < eps) {
        return 0;
    }
    return x < 0 ? -1 : 1;
}

/**
 * @brief (内部函数) 寻找初始的4个不共面的点构成一个四面体, 作为凸包的基础。
 * @return 找到则返回 true, 否则(所有点共面或共线)返回 false。
 */
bool Find(int n) {
    // 找到3个不共线的点
    for (int i = 2; i < n; i++) {
        p3 ndir = cross(p[0] - p[i], p[1] - p[i]);
        if (ndir == p3(0, 0, 0)) continue;
        swap(p[i], p[2]);
        // 找到第4个不共面的点
        for (int j = i + 1; j < n; j++) {
            if (sgn(volume(0, 1, 2, j)) != 0) {
                swap(p[j], p[3]);
                // 添加初始四面体的两个面 (内外侧), 保证初始方向正确
                ins(0, 1, 2);
                ins(0, 2, 1);
                return true;
            }
        }
    }
    return false;
}

mt19937 rng;  // 随机数生成器, 用于打乱点集

/**
 * @brief 主函数, 计算点集 p 的三维凸包。
 * @return 如果成功构建凸包则返回 true, 否则返回 false。
 */
bool solve() {
    face.clear();
    int n = (int)p.size();
    // 随机打乱点集, 这是增量法期望复杂度为 O(n log n) 的关键
    shuffle(p.begin(), p.end(), rng);
    // 找到初始四面体
    if (!Find(n)) return false;
    // 逐个将剩余的点加入凸包
    for (int i = 3; i < n; i++) add(i);
    return true;
}
