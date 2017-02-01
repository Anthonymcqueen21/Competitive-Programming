#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <string>
#include <cmath>

#include <math.h>


typedef std::vector<double> values_t;

double analyze(const std::vector<double>& values)
{
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    double average = sum/values.size();
    std::vector<bool> excluded(values.size(), false);

    std::vector<double> diffs(values.size());
    std::vector<double> dsqrs(values.size());
    for (size_t i = 0; i < values.size(); i++)
    {
        diffs[i] = values[i] - average;
        dsqrs[i] = diffs[i]*diffs[i];
    }
    double range = sqrt(std::accumulate(dsqrs.begin(), dsqrs.end(), 0.0)/(values.size() -1));

    int n = 0;
    std::cerr << "sum: " << sum << std::endl;
    std::cerr << "size: " << values.size() << std::endl;
    std::cerr << "average: " << average << std::endl;
    std::cerr << "range: " << range << std::endl;
    std::cerr << "range/average: " << range/average << std::endl;
    std::cerr << std::endl;
    for (size_t i = 0; i < values.size(); i++)
    {
        if (average - range < values[i] && average + range > values[i])
        {
            n++;
        }
    }

    return range;
}

void scale_to(values_t& values, double length = 1.0)
{
    size_t min_v = 0;
    size_t max_v = 0;
    for (values_t::size_type i = 0; i < values.size(); i++)
    {
        if (values[min_v] > values[i])
        {
            min_v = i;
        }
        if (values[max_v] < values[i])
        {
            max_v = i;
        }
    }

    double min = values[min_v];
    double max = values[max_v] - min;
    for (values_t::iterator i = values.begin(); i != values.end(); i++)
    {
        if (0 != max)
        {
            *i = length*(*i - min)/max;
        } else
        {
            *i = 0;
        }
    }
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int N;
    std::cin >> N;
    std::cin.ignore();

    std::vector<double> counts(N);
    std::vector<double> values(N);
    std::vector<double> v_1(N);
    std::vector<double> n(N);
    std::vector<double> log_n(N);
    std::vector<double> n_log_n(N);
    std::vector<double> n_2(N);
    std::vector<double> n_2_log_n(N);
    std::vector<double> n_3(N);
    std::vector<double> e(N);

    for (int i = 0; i < N; i++)
    {
        std::cin >> counts[i] >> values[i];
        n[i] = counts[i];
        log_n[i] = log(n[i]);
        n_log_n[i] = n[i]*log_n[i];
        n_2[i] = n[i]*n[i];
        n_2_log_n[i] = n_2[i]*log(n[i]);
        n_3[i] = n_2[i]*n[i];
        e[i] = exp(n[i]);
        std::cerr << e[i] << " " << std::endl;
        std::cin.ignore();
    }

    scale_to(values);
    scale_to(v_1);
    scale_to(n);
    scale_to(log_n);
    scale_to(n_log_n);
    scale_to(n_2);
    scale_to(n_2_log_n);
    scale_to(n_3);
    scale_to(e);

    for (values_t::size_type i = 0; i < values.size(); i++)
    {
        v_1[i] = values[i] - v_1[i];
        n[i] = values[i] - n[i];
        log_n[i] = values[i] - log_n[i];
        n_log_n[i] = values[i] - n_log_n[i];
        n_2[i] = values[i] - n_2[i];
        n_2_log_n[i] = values[i] - n_2_log_n[i];
        n_3[i] = values[i] - n_3[i];
        e[i] = values[i] - e[i];
    }

    double O_1 = analyze(v_1);
    double O_n = analyze(n);
    double O_log_n = analyze(log_n);
    double O_n_log_n = analyze(n_log_n);
    double O_n_2 = analyze(n_2);
    double O_n_2_log_n = analyze(n_2_log_n);
    double O_n_3 = analyze(n_3);
    double O_e = analyze(e);

    std::map<double, std::string> result;

    if (std::isfinite(O_1))
    {
        result[O_1] = "O(1)";
    }
    if (std::isfinite(O_n))
    {
        result[O_n] = "O(n)";
    }
    if (std::isfinite(O_log_n))
    {
        result[O_log_n] = "O(log n)";
    }
    if (std::isfinite(O_n_log_n))
    {
        result[O_n_log_n] = "O(n log n)";
    }
    if (std::isfinite(O_n_2))
    {
        result[O_n_2] = "O(n^2)";
    }
    if (std::isfinite(O_n_2_log_n))
    {
        result[O_n_2_log_n] = "O(n^2 log n)";
    }
    if (std::isfinite(O_n_3))
    {
        result[O_n_3] = "O(n^3)";
    }
    if (std::isfinite(O_e))
    {
        result[O_e] = "O(2^n)";
    }

    std::cout.precision(2);
    std::cerr << "O(1): " << O_1 << std::endl;
    std::cerr << "O(n): " << O_n << std::endl;
    std::cerr << "O(log n): " << O_log_n << std::endl;
    std::cerr << "O(n_log_n): " << O_n_log_n << std::endl;
    std::cerr << "O(n_2): " << O_n_2 << std::endl;
    std::cerr << "O(n_2_log_n): " << O_n_2_log_n << std::endl;
    std::cerr << "O(n_3): " << O_n_3 << std::endl;
    std::cerr << "O(e): " << O_e << std::endl;

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    for (std::map<double, std::string>::const_iterator j = result.begin(); j != result.end(); j++)
    {
        std::cerr << j->first << ": " << j->second << std::endl;
    }
    std::map<double, std::string>::const_iterator i = result.begin();
    if (!std::isfinite(i->first))
    {
        i++;
        std::cerr << "result is infinite" << std::endl;
    }
    std::cout << i->second << std::endl;
}
