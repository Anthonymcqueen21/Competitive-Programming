#include <bits/stdc++.h>
using namespace std;

vector<int> solve() {
	int n, k;
	cin >> n >> k;
        vector<int> a(n);
	for (int i = 0; i < n; i++) a[i] = i + 1;

	for (int i = 0; i < k; i++) {
		for (int j = i; j < n; j += k * 2) {
			if (j + k >= n) return{ -1 };
			swap(a[j], a[j + k]);
		}
	}
	
  return a;
}
int main() {
	int T;
	cin >> T;

	while (T--) {
		auto ans = solve();
		for (int i = 0; i < ans.size(); i++) printf("%d ", ans[i]);
		cout << endl;
	}
}
