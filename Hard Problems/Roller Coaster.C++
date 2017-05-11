#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
struct group_t
{
    long long int size;
    int earn;
    int sequence;

    group_t(long long int s): size(s), earn(-1), sequence(0) {}
    group_t(long long int s, int e, int seq): size(s), earn(e), sequence(seq) {}
};
int main()
{
    int L;
    int C;
    int N;
    std::cin >> L >> C >> N;
    std::cin.ignore();
    std::cerr << "L: " << L << "; C: " << C << std::endl;

    std::queue<group_t> groups;
    for (int i = 0; i < N; i++)
    {
        int Pi;
        std::cin >> Pi;
        std::cin.ignore();
        groups.push(group_t(Pi));
        std::cerr << Pi << std::endl;
    }

    long long int result = 0;
    bool used = false;
    for (int i = 0; i < C; i++)
    {
        if (0 <= groups.front().earn && !used)
        {
            std::cerr << "found loop from " << groups.front().sequence << "; earn is " << groups.front().earn << std::endl;
            long long int loop_length = i - groups.front().sequence;
            long long int remain = C - i;
            long long int loops = remain/loop_length;
            long long int earn = result - groups.front().earn;
            std::cerr << "current earn is " << result << ", estimated earn is " << earn << std::endl;
            result += loops*earn;
            std::cerr << "loop length is " << loop_length << ", count is " << loops << std::endl;
            std::cerr << "i (" << i << ") will be set to " << i + loops*loop_length << "/" << C << std::endl;
            std::cerr << "result was set to " << result << std::endl;
            i += loops*loop_length - 1;
            used = true;
            continue;
        }
        long long int rc = 0;
        std::queue<group_t> riders;
        while (!groups.empty() && rc + groups.front().size <= L)
        {
            rc += groups.front().size;
            riders.push(groups.front());
            groups.pop();
        }

        std::cerr << "ride with " << rc << " riders" << std::endl;
        riders.front().sequence = i;
        riders.front().earn = result;
        result += rc;
        while (!riders.empty())
        {
            groups.push(riders.front());
            riders.pop();
        }
    }

    std::cout << result << std::endl;
}
