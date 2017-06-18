#include <iostream>
#include <string>
#include <stdexcept>
#include <list>
using namespace std;
class CCard
{
    int m_value;
    int m_suit;
    public:
        CCard(std::istream& in): m_value(0), m_suit(0)
        {
            std::string card;
            in >> card;
            in.ignore();
            if ('2' <= card[0] && '9' >= card[0])
            {
                m_value = card[0] - '0';
                card = card.substr(1);
            }
            else if ('1' == card[0])
            {
                m_value = 10;
                card = card.substr(2);
            }
            else if ('J' == card[0])
            {
                m_value = 11;
                card = card.substr(1);
            }
            else if ('Q' == card[0])
            {
                m_value = 12;
                card = card.substr(1);
            }
            else if ('K' == card[0])
            {
                m_value = 13;
                card = card.substr(1);
            }
            else if ('A' == card[0])
            {
                m_value = 14;
                card = card.substr(1);
            }
            
            switch (card[0])
            {
                case 'D':
                    m_suit = 1;
                    break;
                    
                case 'H':
                    m_suit = 2;
                    break;
                    
                case 'C':
                    m_suit = 3;
                    break;
                    
                case 'S':
                    m_suit = 4;
                    break;
                    
                default:
                    throw std::runtime_error("Unexpected suit");
            }
        }

        int value() const { return m_value; }

        bool operator<(const CCard& r) const
        {
            return m_value < r.m_value;
        }
};

typedef std::list<CCard> deck_t;

void read_deck(deck_t& deck)
{
	int n;
	std::cin >> n;
	std::cin.ignore();
	
	for (int i = 0; i < n; i++)
	{	
		CCard card(std::cin);
		deck.push_back(card);
	}
}

void dump_deck(const std::string& prefix, const deck_t& d)
{
    std::cerr << prefix << ": ";

    bool first = true;
    for (deck_t::const_iterator i = d.begin(); i != d.end(); i++)
    {
        std::cerr << (first ? "" : ", ") << i->value();
        first = false;
    }
    std::cerr << std::endl;
}

typedef std::pair<int, int> solution_t;
solution_t solve(deck_t f, deck_t s)
{
    int turn = 0;
    
    deck_t* winner;
    do
    {
        winner = NULL;

        dump_deck("deck 1", f);
        dump_deck("deck 2", s);

        // take first cards (fight)
        deck_t::const_iterator c1 = f.begin();
        deck_t::const_iterator c2 = s.begin();
            
        do
        {
            if (*c1 < *c2)
            {
                // second have won
                winner = &s;
            }
            else if (*c2 < *c1)
            {
                // first have won
                winner = &f;
            }
            else
            {
                // equal. war is going to start
                c1++;
                if (f.end() == c1)
                {
                    break;
                }
                c1++;
                if (f.end() == c1)
                {
                    break;
                }
                c1++;
                if (f.end() == c1)
                {
                    break;
                }
                c1++;
                if (f.end() == c1)
                {
                    break;
                }
                
                c2++;
                if (s.end() == c2)
                {
                    break;
                }
                c2++;
                if (s.end() == c2)
                {
                    break;
                }
                c2++;
                if (s.end() == c2)
                {
                    break;
                }
                c2++;
                if (s.end() == c2)
                {
                    break;
                }
            }
        } while (NULL == winner);
        turn++;
        
        std::cerr << "turn #" << turn << std::endl;

        if (NULL != winner)
        {
            // move card to the winner
            deck_t::iterator i = f.begin();
            while (i != c1)
            {
                winner->push_back(*i++);
                f.pop_front();
            }
            winner->push_back(*i);
            f.pop_front();

            i = s.begin();
            while (i != c2)
            {
                winner->push_back(*i++);
                s.pop_front();
            }
            winner->push_back(*i);
            s.pop_front();
        }
    } while (!f.empty() && !s.empty() && NULL != winner);
    
    if (f.empty())
    {
        return solution_t(2, turn);
    }
    else if (s.empty())
    {
        return solution_t(1, turn);
    }
    else if (NULL == winner)
    {
        return solution_t(0, 0);    // PAT
    }
    else
    {
        throw std::runtime_error("Unexpected exit");
    }
    
    throw std::runtime_error("Wrong logic");
}

int main()
{
    try
    {
    	deck_t first;
    	deck_t second;
    	
    	read_deck(first);
    	read_deck(second);

        dump_deck("first player", first);
        dump_deck("second player", second);
    	
    	solution_t s = solve(first, second);
    	std::cerr << "solution has been found" << std::endl;
    	
    	switch (s.first)
    	{
    	    case 1:
    	    case 2:
    	        std::cout << s.first << " " << s.second << std::endl;
    	        break;
    
            default:	        
    	        std::cout << "PAT" << std::endl;
    	}
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
	
	return 0;
}
