for (int s = (1 << r) - 1; s < 1 << n;) {
    int t = s + (s & -s);
    s = (s & ~t) >> __lg(s & -s) + 1 | t;
}