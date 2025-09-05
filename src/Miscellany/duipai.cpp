#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int t = 0;
    while (1) {
        cout << "test: " << t++ << endl;
        system("data.exe > data.in");
        system("std.exe < data.in > std.out");
        system("solve.exe < data.in > solve.out");
        if (system("fc std.out solve.out > diff.log")) {
            cout << "WA\n";
            break;
        }
        cout << "AC\n";
    }
    return 0;
}