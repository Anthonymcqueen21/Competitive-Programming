#include <bits/stdc++.h>

using namespace std;



const long long mod = 1e9 + 7;



vector<long long> F, invF;

long long modpow(long long a, long long b, long long mod) {

	long long ret = 1;

	for (; b > 0; a = a * a % mod, b /= 2) if (b & 1) ret = ret * a % mod;

	return ret;

}

long long modinv(long long a, long long mod) {

	return modpow(a, mod - 2, mod);

}

void initF(int n) {

	F.resize(n, 1);

	invF.resize(n, 1);

	for (int i = 1; i < n; i++) F[i] = i * F[i - 1] % mod;

	invF[n - 1] = modinv(F[n - 1], mod);

	for (int i = n - 2; i >= 0; i--) invF[i] = invF[i + 1] * (i + 1) % mod;

}

long long P(int a, int b) {

	if (a < b || a < 0 || b < 0) return 0;

	return F[a] * invF[a - b] % mod;

}

long long C(int a, int b) {

	if (a < b || a < 0 || b < 0) return 0;

	return P(a, b) * invF[b] % mod;

}

long long H(int a, int b) {

	if (a == 0 && b == 0) return 1;

	return C(a + b - 1, b);

}



int dp[420][220][220]; // pos, open, num



int main() {

	initF(1010101);



	dp[0][0][0] = 1;

	for (int i = 0; i < 410; i++) {

		for (int j = 0; j < 210; j++) {

			for (int k = 0; k < 210; k++) {

				(dp[i + 1][j + 1][k] += dp[i][j][k]) %= mod;

				if (j - 1 >= 0) {

					(dp[i + 1][j - 1][k + (j - 1 == 0)] += dp[i][j][k]) %= mod;

				}

			}

		}

	}



	int T;

	cin >> T;



	while (T--) {

		int x, y;

		cin >> x >> y;



		long long b = C(2 * x, x) * modinv(x + 1, mod) % mod;



		long long ans = 0;

		for (int i = 0; i < 210; i++) {

			(ans += b * C(x * 2 + i, i) % mod * dp[y * 2][0][i]) %= mod;

		}

		(ans *= F[x]) %= mod;

		(ans *= F[y]) %= mod;

		cout << ans << endl;

	}

}
