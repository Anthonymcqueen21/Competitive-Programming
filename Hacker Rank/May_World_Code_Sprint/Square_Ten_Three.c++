#include <bits/stdc++.h>

using namespace std;



const int N = 1 << 22;



string trim_leadzero(string a) {

	int i = a.size() - 1;

	while (i >= 0 && a[i] == '0') i--;

	if (i == -1) return "0";

	a = a.substr(0, i + 1);

	reverse(a.begin(), a.end());

	return a;

}



void increment(string &s, int k) {

	for (int i = k; i < s.size(); i++) {

		s[i]++;

		if (s[i] - '0' == 10) s[i] -= 10;

		else break;

	}



}



void decrement(string &s, int k) {

	for (int i = k; i < s.size(); i++) {

		s[i]--;

		if (s[i] - '0' < 0) s[i] += 10;

		else break;

	}

}



void move_next(string &s, int pos) {

	bool any = false;

	for (int i = 0; i < pos; i++) {

		any |= s[i] != '0';

		s[i] = '0';

	}

	if (any) increment(s, pos);

}



void move_prev(string &s, int pos) {

	for (int i = 0; i < pos; i++) s[i] = '0';

}



bool borrow_flag;



string subtract(string a, string b) {

	int borrow = 0;

	for (int i = 0; i < a.size(); i++) {

		a[i] -= (b[i] - '0') + borrow;

		borrow = a[i] < '0';

		if (a[i] < '0') a[i] += 10;

	}

	borrow_flag = borrow;

	return a;

}



int power2(int d) {

	if (d == 0) return 0;

	return 1 << (d - 1);

}



int main() {

	string L, R;

	cin >> L >> R;



	L = string(N - L.size(), '0') + L;

	R = string(N - R.size(), '0') + R;



	reverse(L.begin(), L.end());

	reverse(R.begin(), R.end());

	decrement(L, 0);



	vector<pair<int, string>> ans_l, ans_r;

	for (int d = 0; d <= 20; d++) {

		string prev_L = L;

		string prev_R = R;



		move_next(L, power2(d + 1));

		move_prev(R, power2(d + 1));



		string tmp = subtract(R, L);

		if (borrow_flag || trim_leadzero(tmp) == "0") {

			string num = trim_leadzero(subtract(prev_R, prev_L).substr(power2(d)));

			if (num != "0") ans_l.emplace_back(d, num);

			break;

		} else {

			string num_l = trim_leadzero(subtract(L, prev_L).substr(power2(d)));

			string num_r = trim_leadzero(subtract(prev_R, R).substr(power2(d)));

			if (num_l != "0") ans_l.emplace_back(d, num_l);

			if (num_r != "0") ans_r.emplace_back(d, num_r);

		}

	}



	reverse(ans_r.begin(), ans_r.end());



	ans_l.insert(ans_l.end(), ans_r.begin(), ans_r.end());

	cout << ans_l.size() << endl;

	for (auto x : ans_l) {

		cout << x.first << " " << x.second << endl;

	}

}
