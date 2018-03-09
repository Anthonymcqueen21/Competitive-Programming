#include <bits/stdc++.h>
using namespace std;

int n, m, q;
int T[100100];

// ***************************************************************
// persistent RBST
// ***************************************************************
struct rbst {
	int size = 1;
	int key;
	rbst *lch = nullptr;
	rbst *rch = nullptr;
	rbst(int key) : key(key) {}
};
void print(rbst *x) {
	if (x == nullptr) return;
	print(x->lch);
	cout << x->key << " ";
	print(x->rch);
}
unsigned xor32() {
	static unsigned z = time(NULL);
	z ^= z << 13; z ^= z >> 17; z ^= z << 5;
	return z;
}
int size(rbst *x) {
	return x == nullptr ? 0 : x->size;
}
rbst *push(rbst *x) {
	x = new rbst(*x);
	x->size = 1 + size(x->lch) + size(x->rch);
	return x;
}
rbst *merge(rbst *x, rbst *y) {
	if (x == nullptr) return y;
	if (y == nullptr) return x;
	if (xor32() % (size(x) + size(y)) < size(x)) {
		x = push(x);
		x->rch = merge(x->rch, y);
		return push(x);
	} else {
		y = push(y);
		y->lch = merge(x, y->lch);
		return push(y);
	}
}
pair<rbst *, rbst *> split(rbst *x, int k) {
	if (!x) return{ nullptr, nullptr };
	x = push(x);
	if (k <= size(x->lch)) {
		auto tmp = split(x->lch, k);
		x->lch = tmp.second;
		return make_pair(tmp.first, push(x));
	} else {
		auto tmp = split(x->rch, k - size(x->lch) - 1);
		x->rch = tmp.first;
		return make_pair(push(x), tmp.second);
	}
}
int lower_bound(rbst *x, int key) {
	if (!x) return 0;
	if (key <= x->key) return lower_bound(x->lch, key);
	return lower_bound(x->rch, key) + size(x->lch) + 1;
}
bool contains(rbst *x, int key) {
	if (!x) return false;
	if (key == x->key) return true;
	if (key < x->key) return contains(x->lch, key);
	return contains(x->rch, key);
}
rbst *insert(rbst *x, int key) {
	int lb = lower_bound(x, key);
	auto p = split(x, lb);

	if (contains(x, key)) {
		x = merge(p.first, p.second);
	} else {
		rbst *n = new rbst(key);
		x = merge(merge(p.first, n), p.second);
	}
	return x;
}
rbst *inorder(rbst *x, rbst *y) {
	if (x == nullptr) return y;
	y = inorder(x->lch, y);
	y = insert(y, x->key);
	y = inorder(x->rch, y);
	return y;
}

// ***************************************************************
// persistent union find 
// ***************************************************************
struct UnionFind {
	struct node {
		node *parent;
		int size = 1;
		int id;
		rbst *st = nullptr;
		node(int id) : id(id) { parent = this; }
	};

	vector<map<int, node *>> uf;

	UnionFind(int n) {
		uf.assign(n, map<int, node *>());
		for (int i = 0; i < n; i++) {
			node *x = new node(i);
			x->st = new rbst(T[i]);
			uf[i][-1] = x;
		}
	}

	node *clone(int sec, node *x) {
		x = new node(*x);
		uf[x->id][sec] = x;
		return x;
	}

	node *get(int x) {
		return uf[x].rbegin()->second;
	}

	node *get(int sec, int x) {
		return prev(uf[x].upper_bound(sec))->second;
	}

	node *root(int sec, node *x) {
		if (x->parent == x) return x;
		return root(sec, get(sec, x->parent->id));
	}

	void merge(int sec, node *x, node *y) {
		x = root(sec, x);
		y = root(sec, y);
		if (x == y) return;
		x = clone(sec, x);
		y = clone(sec, y);

		rbst *st;
		if (size(x->st) > size(y->st)) {
			st = inorder(y->st, x->st);
		} else {
			st = inorder(x->st, y->st);
		}

		x->parent = y->parent = x->size > y->size ? x : y;
		x->size = y->size = x->size + y->size;
		x->st = y->st = st;
	}

	void merge(int sec, int x, int y) {
		merge(sec, get(x), get(y));
	}

	bool query(int sec, int x, int y, int k) {
		node *xx = prev(uf[x].upper_bound(sec))->second;
		node *yy = prev(uf[y].upper_bound(sec))->second;
		xx = root(sec, xx);
		yy = root(sec, yy);
		if (xx != yy) return false;
		return size(xx->st) >= k;
	}
};


int main() {
	cin >> n >> m >> q;
	for (int i = 0; i < n; i++) scanf("%d", &T[i]);

	vector<tuple<int, int, int>> es(m);
	vector<int> ww(m);
	for (int i = 0; i < m; i++) {
		int u, v, w;
		scanf("%d %d %d", &u, &v, &w);
		es[i] = make_tuple(w, u - 1, v - 1);
		ww[i] = w;
	}
	sort(es.begin(), es.end());

	sort(ww.begin(), ww.end());
	ww.erase(unique(ww.begin(), ww.end()), ww.end());

	UnionFind uf(n);
	for (int i = 0; i < m; i++) {
		int w, u, v;
		tie(w, u, v) = es[i];
		w = lower_bound(ww.begin(), ww.end(), w) - ww.begin();
		uf.merge(w, u, v);
	}

	for (int ii = 0; ii < q; ii++) {
		int x, y, k;
		scanf("%d %d %d", &x, &y, &k);
		x--; y--;

		int pass = ww.size();
		int fail = 0;
		while (pass - fail > 1) {
			int mid = (pass + fail) / 2;
			if (uf.query(mid, x, y, k)) {
				pass = mid;
			} else {
				fail = mid;
			}
		}
		if (pass == ww.size()) {
			printf("%d\n", -1);
		} else {
			printf("%d\n", ww[pass]);
		}
	}
}
