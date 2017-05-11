#include <math.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <cstdio>
double get_number(std::string& s)
{
    size_t p = s.find(',');
    if (std::string::npos != p)
    {
        s[p] = '.';
    }
    float result;
    sscanf(s.c_str(), "%f", &result);
    return result*M_PI/180;
}
double _distance(double lat1, double long1, double lat2, double long2)
{
    double x = (long2 - long1)*cos((lat1 + lat2)/2);
    double y = lat2 - lat1;
    double result = sqrt(x*x + y*y)*6371;
    //std::cerr << "lat1 = " << lat1 << "; long1 = " << long1 << std::endl;
    //std::cerr << "lat2 = " << lat2 << "; long2 = " << long2 << std::endl;
    //std::cerr << "x = " << x << "; y = " << y << "; result = " << result << std::endl;
    return result;
}

double _distance(const std::pair<double, double>& a, const std::pair<double, double>& b)
{
    return _distance(a.first, a.second, b.first, b.second);
}

void unquote(std::string& v)
{
    v = v.substr(1, v.length() - 2);
}

typedef std::string edge_t;
typedef std::list<edge_t> edges_list_t;
typedef std::set<std::pair<double, edge_t> > edges_set_t;
typedef std::map<std::string, edges_set_t> graph_t;

bool operator<(const std::pair<double, edge_t>& a, const std::pair<double, edge_t>& b)
{
    return a.first < b.first
        || (a.first == b.first && a.second < b.second);
}

int main()
{
        std::string sa1, sa2;
        getline(std::cin, sa1);
        getline(std::cin, sa2);
        int count;
        std::cin >> count;
        std::cin.ignore();

        // Read nodes
        typedef std::map<std::string, std::pair<double, double> > nodes_t;
        typedef std::map<std::string, std::string> node2name_t;

        nodes_t nodes;
        node2name_t n2n;

        graph_t graph;
        std::map<std::string, double> lengths;
        for (int i = 0; i < count; i++)
        {
            std::string line;
            getline(std::cin, line);

            std::vector<std::string> parts(9);
            for (int i = 0; i < 9 - 1; i++)
            {
                size_t pos = line.find(',');
                if (pos == std::string::npos)
                {
                    //std::cout << "Invalid format: " << line << std::endl;
                    return -1;
                }
                parts[i] = line.substr(0, pos);
                line = line.substr(pos + 1);
                //std::cerr << "Part #" << i << ": " << parts[i] << std::endl;
            }
            parts[8] = line;
            unquote(parts[1]);
            n2n[parts[0]] = parts[1];
            nodes[parts[0]] = nodes_t::mapped_type(get_number(parts[3]), get_number(parts[4]));
            graph[parts[0]];
            lengths[parts[0]] = -1.0;
            //cerr << parts[1] << ": " << get_number(parts[3]) << ", " << get_number(parts[4]) << endl;
            //std::cerr << parts[0] << ": " << nodes[parts[0]].first << ", " << nodes[parts[0]].second << std::endl;
        }

        // Read edges
        std::cin >> count;
        std::cin.ignore();
        std::map<std::string, std::string> rgraph;
        for (int i = 0; i < count; i++)
        {
            std::string n1, n2;
            std::cin >> n1 >> n2;
            std::cin.ignore();
            if (n1 == "StopArea:AMER")
            {
                std::cerr << n1 << " " << n2 << std::endl;
            }
            graph[n1].insert(std::pair<double, edge_t>(_distance(nodes[n1], nodes[n2]), n2));
        }

        std::set<std::string> processed;
        edges_list_t to_visit;
        to_visit.push_back(sa1);
        processed.insert(sa1);
        lengths[sa1] = 0.0;
        while (!to_visit.empty())
        {
            std::string e = to_visit.front();
            for (edges_list_t::const_iterator i = to_visit.begin(); i != to_visit.end(); i++)
            {
                if (lengths[e] > lengths[*i])
                {
                    e = *i;
                }
            }
            to_visit.remove(e);
            double w = lengths[e];
            static int C = 1;
            std::cerr << C++ << ". check node " << n2n[e] << ": " << nodes[e].first << ", " << nodes[e].second << std::endl;
            for (edges_set_t::const_iterator i = graph[e].begin(); i != graph[e].end(); i++)
            {
                if (processed.end() == processed.find(i->second))
                {
                    to_visit.push_back(i->second);
                    processed.insert(i->second);
                }
                double d = i->first;
                std::cerr << "w = " << w << "\td = " << d << "\tcurrent (" << n2n[i->second] << "): " << lengths[i->second] << std::endl;
                if (-1.0 == lengths[i->second]
                    || w + d < lengths[i->second])
                {
                    lengths[i->second] = w + d;
                    rgraph[i->second] = e;
                    std::cerr << "update path" << std::endl;
                } else
                {
                    std::cerr << "skip" << std::endl;
                }
            }
        }

        for (std::map<std::string, double>::const_iterator i = lengths.begin(); i != lengths.end(); i++)
        {
            std::cerr << n2n[i->first] << ": " << i->second << std::endl;
        }

        // Build path
        if (-1.0 == lengths[sa2])
        {
            std::cout << "IMPOSSIBLE" << std::endl;
        } else
        {
            edges_list_t result;
            std::string n = sa2;
            result.push_front(sa2);
            while (sa1 != n)
            {
                std::string nmin = rgraph[n];
                result.push_front(nmin);
                n = nmin;
                std::cerr << n2n[nmin] << ":\t" << nmin << std::endl;
            }

            for (edges_list_t::const_iterator i = result.begin(); i != result.end(); i++)
            {
                std::cout << n2n[*i] << std::endl;
            }
        }

        return 0;
}
