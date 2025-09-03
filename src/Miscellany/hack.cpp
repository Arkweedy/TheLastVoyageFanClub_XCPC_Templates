#pragma GCC optimize("Ofast,unroll-loops")

int read() {
    char c = getchar();
    int f = 1, x = 0;
    while (c < 48 || c > 57) {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c >= 48 && c <= 57) {
        x = 10 * x + c - 48;
        c = getchar();
    }
    return x * f;
}
void write(int x) {
    if (x < 0) putchar('-'), x = -x;
    if (x > 9) write(x / 10);
    putchar(x % 10 + 48);
}