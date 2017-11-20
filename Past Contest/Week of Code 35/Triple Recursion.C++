#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


int main() {
    int n, m, k;
    cin >> n >> m >> k;
    int a[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 && j == 0) {
                a[i][j] = m;
            }
            else if (i == j) {
                a[i][j] = a[i - 1][j - 1] + k;
            }
            else if(i > j)
                a[i][j] = a[i - 1][j] - 1;
            else if(i < j)
                a[i][j] = a[i][j - 1] - 1;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << a[i][j] << ' ';
        }
        std::cout << '\n';
    }
    return 0;
}
