struct Q {
    int l, r, id;
};

bool cmp(const Q& a, const Q& b) {
    if (be[a.l] != be[b.l]) {
        return be[a.l] < be[b.l];
    }
    return a.r < b.r;
}

void solve() {

    for (const auto& query : q) {
        // 情况1: 左右端点在同一个块内，直接暴力求解
        if (belo[query.l] == belo[query.r]) {
            continue;
        }

        // 情况2: 左端点进入了一个新的块，重置所有状态和指针
        if (be[query.l] != last) {
            last = be[query.l];
            // 将 r 和 l 重置到当前块的右边界
            r = R[last];
            l = r + 1;
        }

        // 移动右指针
        while (r < query.r) {
            add(++r);
        }

        // 在移动左指针前，保存当前 [l, r] 区间的答案
        int pre = l; // 记录移动前左指针的位置，便于回滚

        // 临时移动左指针
        while (l > query.l) {
            add(--l);
        }

        // 计算答案

        // 回滚左指针的移动，恢复到 [pre, r] 的状态
        while (l < pre) {
            del(l++);
        }
        // 恢复之前保存的状态变量
    }
}