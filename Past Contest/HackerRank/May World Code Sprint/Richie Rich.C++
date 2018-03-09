#include <iostream>
#include <vector>
#include <cassert>
#define repeat(i,n) for (int i=0; (i) < (n); ++(i))
using namespace std;
int main() {
    int n, k; cin >> n >> k;
    string t; cin >> t;
    const string s = t; // original print
    bool is_palindromic = true;
    repeat (i,n/2) if (t[i] != t[n-1-i]) {
        if (not k) {
            is_palindromic = false;
            break;
        } else {
            -- k;
            t[i] = t[n-1-i] = max(t[i], t[n-1-i]);
        }
    }
    repeat (i,n/2) {
        if (not k) break;
        if (t[i] == '9') continue;
        if (s[i] != s[n-1-i]) ++ k; // rerouting
        if (k >= 2) {
            -- k;
            -- k;
            t[i] = t[n-1-i] = '9';
        }
    }
    if (k and t[n/2] != '9') {
        --k;
        t[n/2] = '9';
    }
    assert (k >= 0);
    cout << (is_palindromic ? t : "-1") << endl;
    return 0;
}
