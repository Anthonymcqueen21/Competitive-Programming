#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
class CGraph
{
class CNode
    {
        int m_weight;
        int m_next1;
        int m_next2;
        public:
            CNode(): m_weight(0), m_next1(0), m_next2(0) {}
            CNode(int weight, int next1, int next2): m_weight(weight), m_next1(next1), m_next2(next2) {}
            int weight() const { return m_weight; }
            int top() const { return m_next1; }
            int right() const { return m_next2; }
    };
    
    typedef std::vector<CNode> building_t;
    typedef std::vector<int> weights_t;
    int m_count;
    building_t m_building;
    mutable weights_t m_weights;
    public:
        CGraph(int size): m_count(size), m_building(size), m_weights(size, -1)
        {
        }
        void add_node(int number, int weight, int top, int right)
        {
            m_building[number] = CNode(weight, top, right);
        }
        void dump() const
        {
            for (size_t i = 0; i < m_building.size(); i++)
            {
                const CNode r = m_building[i];
                std::cerr << i << ": " << r.weight() << " " << r.top() << "/" << r.right() << std::endl;
            }
        }
        
        int solve(int room) const
        {
            if (-1 != m_weights[room])
            {
                return m_weights[room];
            }
            const CNode& n = m_building[room];
            int result = n.weight();
            std::cerr << "tn " << room << " " << n.top() << " " << n.right() << std::endl;
            if (0 == n.top() && 0 != n.right())
            {
                result += solve(n.right());
            } else if (0 != n.top() && 0 == n.right())
            {
                result += solve(n.top());
            } else if (0 != n.top() && 0 != n.right())
            {
                result += std::max(solve(n.top()), solve(n.right()));
            } else  // both is zero
            {
                // nothing
            }
            std::cerr << "fr " << room << " " << result << std::endl;
            m_weights[room] = result;
            return result;
        }
        
        int solution() const
        {
            return solve(0);
        }
};
int main()
{
	int size;
	std::cin >> size;
	std::cin.ignore();
	
	CGraph g(size);
	for (int i = 0; i < size; i++)
	{
	    int n, w;
	    std::string t, r;
	    int ti, ri;
	    
	    std::cin >> n >> w >> t >> r;
	    std::cin.ignore();
	    
	    if (t == "E")
	    {
	        ti = 0;
	    }
	    else
	    {
	        ti = atoi(t.c_str());
	    }
	    
	    if (r == "E")
	    {
	        ri = 0;
	    }
	    else
	    {
	        ri = atoi(r.c_str());
	    }
	    
	    std::cerr << n << ": " << w << " " << t << "/" << r << std::endl;
	    
		g.add_node(n, w, ti, ri);
	}
	std::cerr << std::endl;
	g.dump();
	std::cout << g.solution() << std::endl;
	return 0;
}
