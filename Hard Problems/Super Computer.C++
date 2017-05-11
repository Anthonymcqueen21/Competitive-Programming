#include <iostream>
#include <set>
#include <list>
typedef std::pair<int, int> node_t;
typedef std::set<node_t> nodes_t;
bool operator<(const node_t& a, const node_t& b)
{
    return a.first < b.first
        || (a.first == b.first && a.second < b.second);
}
int main()
{
        int n;
        std::cin >> n;
        std::cin.ignore();

        nodes_t nodes;
        for (int i = 0; i < n; i++)
        {
            int start;
            int length;
                std::cin >> start >> length;
                std::cin.ignore();
        nodes.insert(node_t(start, length));
        }

    std::list<node_t> nl;
    for (nodes_t::const_iterator i = nodes.begin(); i != nodes.end(); i++)
    {
        nodes_t::const_iterator j = i;

        if (!nl.empty())
        {
            if (nl.back().first + nl.back().second > i->first)  // skip this part
            {
                continue;
            }
        }
        bool skip = false;
        while (j != nodes.end() && j->first < i->first + i->second)
        {
            if (j->first + j->second < i->first + i->second)    // found lesser. skip this. will get it lesser
            {
                skip = true;
                break;
            }
            j++;
        }
        if (!skip)
        {
            nl.push_back(*i);
        }
    }

        std::cout << nl.size() << std::endl;

        return 0;
}
