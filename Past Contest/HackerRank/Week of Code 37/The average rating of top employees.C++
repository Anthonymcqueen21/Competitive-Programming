#include <cmath>
#include <iostream>

int main() {
    int n, rating, n_above_90 = 0;
    float sum = 0;

    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> rating;
        if (rating < 90) {
            continue;
        }

        sum += rating;
        ++n_above_90;
    }

    const auto average = sum / n_above_90;
    printf("%.2f\n", roundf(average * 100) / 100);
    return 0;
}
