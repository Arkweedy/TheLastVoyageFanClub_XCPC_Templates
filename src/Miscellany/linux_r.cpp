#include <bits/stdc++.h>

using namespace std;

int main(int argc, char* argv[]) {
    string s = argv[1];
    string cpp = s + ".cpp";
    string exe = s;
    string out = s + ".out";
    string err = s + ".err";

    string cp =
        "g++ -std=c++20 -g -O2 "
        "-fsanitize=address " +
        cpp + " -o " + exe;

    if (system(cp.c_str()) != 0) {
        cerr << "Compile Failed!" << endl;
        return 1;
    } else {
        system("clear");
        cerr << "Compile Success!" << endl;
    }

    string run = "./" + exe;
    if (argc > 2) {
        run += " < " + string(argv[2]);
    }
    run += " > " + out + " 2> " + err;

    system(run.c_str());

    cout << "\n---Output---\n";
    system(("cat " + out).c_str());
    cout << "\n--------------------------\n";

    cout << "\n---Error---\n";
    system(("cat " + err).c_str());
    cout << "\n--------------------------\n";

    return 0;
}