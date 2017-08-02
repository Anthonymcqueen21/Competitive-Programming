#include <bits/stdc++.h>
using namespace std;
int n;
int c[101010];
vector<int> g[101010];
int sub[202020];
multimap<int, int> mp[101010];
long long dp_down[101010];
long long dp_up[101010];
long long dp_cost[101010];
vector<int> children[202020];
int parent[202020];
int color_sub[202020];
int super_sub[202020];
void dfs(int curr, int prev) {
	sub[curr] = 1;
	dp_down[curr] = 1;

	for (int next : g[curr]) if (next != prev) {
		dfs(next, curr);
		sub[curr] += sub[next];

		long long cost = dp_down[next] + sub[next];
		for (auto it = mp[next].find(c[curr]); it != mp[next].end() && it->first == c[curr]; it++) {
			cost -= sub[it->second];
		}
		dp_down[curr] += cost;
		dp_cost[next] = cost;

		for (auto it = mp[next].find(c[curr]); it != mp[next].end() && it->first == c[curr]; it++) {
			int x = it->second;
			children[curr].push_back(next + n);
			parent[next + n] = curr;
			children[next + n].push_back(x);
			parent[x] = next + n;
		}

		if (mp[curr].size() < mp[next].size()) swap(mp[curr], mp[next]);
		mp[curr].insert(mp[next].begin(), mp[next].end());
		mp[next].clear();
	}

	mp[curr].erase(c[curr]);
	mp[curr].emplace(c[curr], curr);
}

void dfs2(int curr, int prev) {
	if (prev != -1) {
		dp_up[curr] += dp_up[prev] + dp_down[prev] - dp_cost[curr] + (n - sub[curr]);
		if (parent[curr] != -1) {
			dp_up[curr] -= color_sub[parent[curr]] - sub[curr];
			dp_up[curr] -= n - sub[parent[curr] - n];
		} else {
			dp_up[curr] -= super_sub[c[curr]] - sub[curr];
		}
	}

	for (int next : g[curr]) if (next != prev) dfs2(next, curr);
}

int main() {
	cin >> n;

	for (int i = 0; i < n; i++) scanf("%d", &c[i]);
	for (int i = 0; i < n - 1; i++) {
		int u, v;
		scanf("%d %d", &u, &v);
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}

	memset(parent, -1, sizeof(parent));
	dfs(0, -1);
	for (auto kv : mp[0]) {
		super_sub[kv.first] += sub[kv.second];
	}

	for (int i = 0; i < n * 2; i++) {
		for (int j : children[i]) color_sub[i] += sub[j];
	}
	dfs2(0, -1);

	for (int i = 0; i < n; i++) {
		printf("%lld\n", dp_down[i] + dp_up[i]);
	}
}
