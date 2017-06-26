#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <assert.h>
using namespace std;
class CField
{
    public:
        enum ECell
        {
            EC_NONE = ' ',
            EC_WALL = '#',
            EC_OBSTACLE = 'X',
            EC_BENDER = '@',
            EC_EXIT = '$',
            EC_SOUTH = 'S',
            EC_EAST = 'E',
            EC_NORTH = 'N',
            EC_WEST = 'W',
            EC_BEER = 'B',
            EC_INVERTOR = 'I',
            EC_TELEPORT = 'T'
        };
        
        CField(): _width(0), _height(0)
        {
            prio.push_back(EP_SOUTH);
            prio.push_back(EP_EAST);
            prio.push_back(EP_NORTH);
            prio.push_back(EP_WEST);
            
            inv_prio.push_back(EP_WEST);
            inv_prio.push_back(EP_NORTH);
            inv_prio.push_back(EP_EAST);
            inv_prio.push_back(EP_SOUTH);
        }

        void read()
        {
            cin >> _height >> _width;
            cin.ignore();

            int teleports = 0;
            std::vector<pos_t> t(2);
            field.resize(_width, row_t(_height, EC_NONE));
            for (size_t i = 0; i < _height; i++)
            {
                std::string line;
                std::getline(cin, line);
                //cerr << line.length() << ", " << _width << ": " << line << endl;
                assert(line.length() == _width);

                for (size_t j = 0; j < _width; j++)
                {
                    field[j][i] = static_cast<ECell>(line[j]);
                    if (EC_BENDER == field[j][i])
                    {
                        bender = pos_t(j, i);
                    } else if (EC_EXIT == field[j][i])
                    {
                        _exit = pos_t(j, i);
                    } else if (EC_BEER == field[j][i])
                    {
                        beer.push_back(pos_t(j, i));
                    } else if (EC_TELEPORT == field[j][i])
                    {
                        if (teleports == 0)
                        {
                            t[0] = pos_t(j, i);
                            teleports++;
                        } else if (1 == teleports)
                        {
                            t[1] = pos_t(j, i);
                            teleports++;
                        } else
                        {
                            assert(false);
                        }
                    }
                }
            }
            assert(0 == teleports || 2 == teleports);
            if (2 == teleports)
            {
                teleport[t[0]] = t[1];
                teleport[t[1]] = t[0];
            }
        }
        
        typedef std::list<std::string> path_t;
        path_t get_solution()
        {
            //dump_field();
            path_t result;
            SState s(EP_SOUTH, false, false);
            while (passed.end() == passed.find(cell_state_t(bender, s))
                && _exit != bender)
            {
                passed.insert(cell_state_t(bender, s));
                bool should_clear = step(bender, s, result);
                if (should_clear)
                {
                    passed.clear();
                }
            }
            if (_exit != bender)
            {
                result.clear();
                result.push_back("LOOP");
            }
            
            return result;
        }
        
        path_t get_wrong_solution() const
        {
            weighted_field_t w(_height, std::vector<int>(_width, 0));
            w[bender.first][bender.second] = 1;
            bool step_taken;
            bool drunken = false;
            do
            {
                weighted_field_t w2 = w;
                step_taken = false;
                for (size_t x = 1; x < _width - 1; x++)
                {
                    for (size_t y = 1; y < _height - 1; y++)
                    {
                        if (0 != w2[x][y])
                        {
                            w2[x][y]++;
                        }
                    }
                }
                
                for (size_t x = 1; x < _width - 1; x++)
                {
                    for (size_t y = 1; y < _height - 1; y++)
                    {
                        if (0 != w[x][y])
                        {
                            step_taken = try_step_to(w, w2, x - 1, y, drunken) || step_taken;
                            step_taken = try_step_to(w, w2, x + 1, y, drunken) || step_taken;
                            step_taken = try_step_to(w, w2, x, y - 1, drunken) || step_taken;
                            step_taken = try_step_to(w, w2, x, y + 1, drunken) || step_taken;
                        }
                    }
                }
                
                if (step_taken)
                {
                    w = w2;
                }
                //dump_field();
                //dump_weights(w);
                //cerr << endl;
            } while (step_taken);
            
            path_t result = build_path(w, bender.first, bender.second, _exit.first, _exit.second);
            return result;
        }
        
        void solve()
        {
            path_t solution = get_solution();
            for (path_t::const_iterator i = solution.begin(); i != solution.end(); i++)
            {
                //cerr << *i << endl;
                cout << *i << endl;
            }
        }

    private:
        typedef std::vector<ECell> row_t;
        typedef std::vector<row_t> field_t;
        typedef std::pair<int, int> pos_t;
        typedef std::list<pos_t> pos_list_t;
        typedef std::vector<std::vector<int> > weighted_field_t;
        typedef std::map<pos_t, pos_t> teleport_t;

        enum EPriority
        {
            EP_SOUTH,
            EP_EAST,
            EP_NORTH,
            EP_WEST
        };
        typedef std::list<EPriority> priority_list_t;

        struct SState
        {
            EPriority direction;
            bool inverted;
            bool drunken;
            
            SState(EPriority dir, bool i, bool d): direction(dir), inverted(i), drunken(d) {}
        };
        
        typedef std::pair<pos_t, SState> cell_state_t;
        typedef std::set<cell_state_t> passed_t;
        
        friend bool operator<(const CField::cell_state_t& a, const CField::cell_state_t& b);
        friend bool operator<(const CField::pos_t& a, const CField::pos_t& b);

        void dump_field() const
        {
            for (size_t y = 0; y < _height; y++)
            {
                for (size_t x = 0; x < _width; x++)
                {
                    cerr << static_cast<char>(field[x][y]);
                }
                cerr << endl;
            }
        }
        
        void dump_weights(const weighted_field_t& w) const
        {
            streamsize s = cerr.width();
            for (size_t y = 0; y < w[0].size(); y++)
            {
                for (size_t x = 0; x < w.size(); x++)
                {
                    cerr.width(2);
                    cerr << w[x][y] << " ";
                }
                cerr << endl;
            }
            cerr.width(s);
        }

        bool step(pos_t& b, SState& s, path_t& path)
        {
            bool result = false;
            bool step_taken = false;
            priority_list_t::const_iterator i = s.inverted ? inv_prio.begin() : prio.begin();
            
            do
            {
                int tx = b.first;
                int ty = b.second;
                
                switch (s.direction)
                {
                    case EP_EAST:
                        tx++;
                        break;
                        
                    case EP_NORTH:
                        ty--;
                        break;
                        
                    case EP_WEST:
                        tx--;
                        break;
                        
                    case EP_SOUTH:
                        ty++;
                        break;
                }
                if (EC_WALL == field[tx][ty]
                    || (EC_OBSTACLE == field[tx][ty] && !s.drunken))
                {
                    s.direction = *(i++);
                } else
                {
                    step_taken = true;
                    switch (s.direction)
                    {
                        case EP_EAST:
                            path.push_back("EAST");
                            break;
                            
                        case EP_NORTH:
                            path.push_back("NORTH");
                            break;
                            
                        case EP_WEST:
                            path.push_back("WEST");
                            break;
                            
                        case EP_SOUTH:
                            path.push_back("SOUTH");
                            break;
                    }
                    //std::cerr << path.back() << " (" << tx << ", " << ty << ")" << std::endl;
                    b = pos_t(tx, ty);
                    if (EC_OBSTACLE == field[tx][ty] && s.drunken)
                    {
                        field[tx][ty] = EC_NONE;
                        //s.drunken = false;
                        result = true;
                    }
                }
            } while (!step_taken);
            
            switch (field[b.first][b.second])
            {
                case EC_BEER:
                    s.drunken = !s.drunken;
                    break;
                    
                case EC_INVERTOR:
                    s.inverted = !s.inverted;
                    break;
                    
                case EC_TELEPORT:
                {
                    teleport_t::const_iterator i = teleport.find(b);
                    assert(teleport.end() != i);
                    b = teleport[b];
                    break;
                }
                    
                case EC_SOUTH:
                    s.direction = EP_SOUTH;
                    break;
                    
                case EC_EAST:
                    s.direction = EP_EAST;
                    break;
                    
                case EC_NORTH:
                    s.direction = EP_NORTH;
                    break;
                    
                case EC_WEST:
                    s.direction = EP_WEST;
                    break;

                default:
                    /* do nothing */;
            }
            return result;
        }
        
        path_t build_path(weighted_field_t& w, size_t fx, size_t fy, size_t tx, size_t ty) const
        {
            path_t result;
            if (0 == w[tx][ty])
            {
                return result;
            }
            dump_field();
            dump_weights(w);
            while (tx != fx || ty != fy)
            {
                //cerr << "(" << fx << ", " << fy << ") -> (" << tx << ", " << ty << ")" << endl;
                if (w[tx - 1][ty] > w[tx][ty])
                {
                    result.push_front("EAST");
                    tx--;
                }
                else if (w[tx + 1][ty] > w[tx][ty])
                {
                    result.push_front("WEST");
                    tx++;
                }
                else if (w[tx][ty - 1] > w[tx][ty])
                {
                    result.push_front("SOUTH");
                    ty--;
                }
                else if (w[tx][ty + 1] > w[tx][ty])
                {
                    result.push_front("NORTH");
                    ty++;
                }
                else
                {
                    cerr << "Broken logic" << endl;
                    //exit(1);
                }
            }
            
            return result;
        }
        
        bool try_step_to(weighted_field_t& w, weighted_field_t& w2, size_t x, size_t y, bool drunken) const
        {
            bool result = false;
            if (0 == w[x][y])
            {
                if (EC_NONE == field[x][y]
                    || EC_BEER == field[x][y]
                    || (EC_OBSTACLE == field[x][y] && drunken)
                    || EC_EXIT == field[x][y])
                {
                    w2[x][y] = 1;
                    result = true;
                }
            }
            return result;
        }
        
        size_t _width;
        size_t _height;
        field_t field;
        pos_t bender;
        pos_t _exit;
        passed_t passed;
        priority_list_t prio;
        priority_list_t inv_prio;
        teleport_t teleport;
        
        pos_list_t beer;
};

bool operator<(const CField::pos_t& a, const CField::pos_t& b)
{
    return a.first < b.first
        || (a.first == b.first && a.second < b.second);
}

bool operator<(const CField::cell_state_t& a, const CField::cell_state_t& b)
{
    return a.first.first < b.first.first
        || (a.first.first == b.first.first && a.first.second < b.first.second)
        || (a.first.first == b.first.first && a.first.second == b.first.second
            && static_cast<int>(a.second.direction) < static_cast<int>(b.second.direction))
        || (a.first.first == b.first.first && a.first.second == b.first.second
            && static_cast<int>(a.second.direction) == static_cast<int>(b.second.direction)
            && static_cast<int>(a.second.drunken) < static_cast<int>(b.second.drunken))
        || (a.first.first == b.first.first && a.first.second == b.first.second
            && static_cast<int>(a.second.direction) == static_cast<int>(b.second.direction)
            && static_cast<int>(a.second.drunken) == static_cast<int>(b.second.drunken)
            && static_cast<int>(a.second.inverted) < static_cast<int>(b.second.inverted));
}

int main()
{
    CField field;

    field.read();
    field.solve();

    return 0;
}
