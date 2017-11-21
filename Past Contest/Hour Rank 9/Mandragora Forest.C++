#include <stdio.h>
#include <algorithm>
using namespace std;
int N;
int A[100007];
long long SUM;
inline void init()
{
        int i;
	scanf("%d", &N);
	SUM = 0;
	for (i = 1; i <= N; i++)
	{
		scanf("%d", &A[i]);
		SUM += A[i];
	}

	sort(A + 1, A + N + 1);
}

inline long long solve()
{
	long long prev = 0, curr;
	int i;

	for (i = 1; i <= N; i++)
	{
		curr = SUM * i;
		if (curr >= prev)
			prev = curr;
		else
			return prev;
		SUM -= A[i];
	}

	return prev;
}

int main()
{
	int T;

//	freopen("input.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);

	scanf("%d", &T);

	while (T--)
	{
		init();
		printf("%lld\n", solve());
	}

	return 0;
}
