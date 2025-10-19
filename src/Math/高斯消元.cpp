#define eps 1e-7
int n;
long double a[N][N];
int guess()  // 只接受方阵，0：无解，1：有解
{
    for (int i = 1; i <= n; i++) {
        for (int j = i; j <= n; j++)
            if (abs(a[j][i]) > eps) {
                swap(a[i], a[j]);
                break;
            }
        if (abs(a[i][i]) < eps) {
            return 0;
        }
        for (int j = n + 1; j >= i; j--) a[i][j] /= a[i][i];
        for (int j = 1; j <= n; j++)
            if (i != j) {
                for (int k = n + 1; k >= i; k--) a[j][k] -= a[i][k] * a[j][i];
            }
    }
    return 1;
}
