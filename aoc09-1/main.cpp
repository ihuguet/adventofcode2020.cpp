#include <iostream>
#include <string>
#include <deque>

int main () {
    std::istream &input = std::cin;
    std::deque<uint_least64_t> previous_nums;
    uint_least64_t num;

    for (int i = 0; i < 25 && input >> num; i++) {
        previous_nums.push_back(num);
    }

    while (input >> num) {
        bool ok = false;
        for (auto it1 = previous_nums.cbegin(); !ok && it1 != previous_nums.cend(); it1++) {
            for (auto it2 = previous_nums.cbegin(); !ok && it2 != previous_nums.cend(); it2++) {
                if (it1 != it2 && *it1 + *it2 == num) 
                    ok = true;
            }
        }
        if (!ok) {
            std::cout << "Found wrong number " << num << std::endl;
            return 0;
        }
        previous_nums.pop_front();
        previous_nums.push_back(num);
    }

    std::cerr << "Wrong number not found" << std::endl;
    return 1;
}

