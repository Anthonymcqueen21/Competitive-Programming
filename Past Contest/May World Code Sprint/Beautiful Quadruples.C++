#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#define repeat(i,n) for (int i = 0; (i) < (n); ++(i))
#define repeat_from(i,m,n) for (int i = (m); (i) < (n); ++(i))
typedef long long ll;
using namespace std;
int main() {
    array<int,4> a; repeat (i,4) cin >> a[i];
    sort(a.begin(), a.end());
    vector<ll> cnt(pow(2,ceil(log2(a[3]+1))));
    ll acc = 0;
    repeat_from (y,1,a[2]+1) {
        repeat_from (z,y,a[3]+1) {
            cnt[y^z] += 1;
            acc += 1;
        }
    }
    ll ans = 0;
    repeat_from (x,1,a[1]+1) {
        repeat_from (w,1,min(a[0],x)+1) {
            ans += acc - cnt[w^x];
        }
        int y = x;
        repeat_from (z,x,a[3]+1) {
            cnt[y^z] -= 1;
            acc -= 1;
        }
    }
    cout << ans << endl;
    return 0;
}
