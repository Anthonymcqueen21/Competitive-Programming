#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
using namespace std;

int main() {
    int n, f = 0;
    long minV = 1000000000;
    string minS;
    string vv;
    string ss;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> ss >> vv;
        int j = count(vv.begin(), vv.end(), '4');
        int k = count(vv.begin(), vv.end(), '7');
        if (j == k && (j + k) == vv.length()) {
            stringstream geek(vv);
            long g = 0;
            geek >> g;
            if(g < minV)
            {
            minV = g;
            minS = ss;
        }
        f = 1;
        }
    }
    if(f == 1)
    std::cout << minS << '\n';
    else
    std::cout << -1 << '\n';
    return 0;
}
