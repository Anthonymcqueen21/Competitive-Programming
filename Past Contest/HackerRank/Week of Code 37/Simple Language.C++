#include <algorithm>
#include <string>
#include <iostream>

using ll = long long;

int main() {
    ll n, value, max_value = 0;
    std::string operation;

    std::cin >> n;
    while (n-- > 0) {
        std::cin >> operation >> value;
        if (operation == "add") {
            if (value < 0) {
                continue;
            }

            max_value += value;
        } else if (operation == "set") {
            max_value = std::max(max_value, value);
        }
    }

    std::cout << max_value << std::endl;
    return 0;
}
