#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>

constexpr int CUPS_NUMBER = 1'000'000;
constexpr int ROUNDS_NUMBER = 10'000'000;
using iterator_t = std::list<int>::iterator;

static inline iterator_t circular_next (std::list<int> &list, iterator_t current, size_t count = 1) {
    iterator_t next = current;
    while (count-- > 0)
        next = std::next(next) != list.end() ? std::next(next) : list.begin();
    return next;
}

static inline bool exists_in_list(const std::list<int> &cups, int num) {
    return std::find(cups.cbegin(), cups.cend(), num) != cups.cend();
}

int main () {
    std::list<int> cups {1, 5, 6, 7, 9, 4, 8, 2, 3};
    iterator_t current_cup = cups.begin();

    std::vector<iterator_t> cupsPtrs;
    cupsPtrs.reserve(CUPS_NUMBER);
    cupsPtrs.resize(cups.size());
    while (current_cup != cups.end()) {
        cupsPtrs[*current_cup - 1] = current_cup;
        current_cup++;
    }

    for (int i = cups.size() + 1; i <= CUPS_NUMBER; i++) {
        cups.push_back(i);
        cupsPtrs.push_back(std::prev(cups.end()));
    }

    current_cup = cups.begin();
    for (int i = 0; i < ROUNDS_NUMBER; i++) {
        std::list<int> extracted_cups;
        for (int j = 0; j < 3; j++) {
            auto next_cup = circular_next(cups, current_cup);
            extracted_cups.splice(extracted_cups.end(), cups, next_cup);
        }

        int insertion_cup_num = *current_cup - 1;
        while (insertion_cup_num <= 0 || exists_in_list(extracted_cups, insertion_cup_num)) {
            if (--insertion_cup_num <= 0)
                insertion_cup_num = CUPS_NUMBER;
        }
        iterator_t insertion_point = cupsPtrs[insertion_cup_num - 1];

        cups.splice(std::next(insertion_point), extracted_cups);
        
        current_cup = circular_next(cups, current_cup);
    }

    long cup_next_to_1 = *circular_next(cups, cupsPtrs[0]);
    long cup_next_next_to_1 = *circular_next(cups, cupsPtrs[0], 2);
    std::cout << "Cups next to cup #1: " << cup_next_to_1 << " " << cup_next_next_to_1 << "\n";
    std::cout << "Result: " << (cup_next_to_1 * cup_next_next_to_1) << std::endl;
}
