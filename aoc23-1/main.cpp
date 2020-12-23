#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>

using iterator_t = std::list<int>::iterator;
static inline iterator_t circular_next (std::list<int> &list, iterator_t current) {
    auto next = std::next(current);
    return next == list.end() ? list.begin() : next;
}

static inline std::string cups_str (const std::list<int> &cups) {
    std::string str;
    for (int cup : cups) str += std::to_string(cup);
    return str;
}

int main () {
    std::list<int> cups {1, 5, 6, 7, 9, 4, 8, 2, 3};
    auto current_cup = cups.begin();

    for (int i = 0; i < 100; i++) {
        std::cout << "-- move " << (i+1) << " --\n";
        std::cout << "cups: " << cups_str(cups) << "\n";

        std::list<int> extracted_cups;
        for (int j = 0; j < 3; j++) {
            auto next_cup = circular_next(cups, current_cup);
            extracted_cups.splice(extracted_cups.end(), cups, next_cup);
        }
        std::cout << "extracted: "<< cups_str(extracted_cups) << "\n";

        iterator_t insertion_point = cups.end();
        for (int j = *current_cup - 1; insertion_point == cups.end(); j--) {
            if (j >= 0) 
                insertion_point = std::find(cups.begin(), cups.end(), j);
            else
                insertion_point = std::max_element(cups.begin(), cups.end());
        }
        std::cout << "destination: " << *insertion_point << "\n\n";

        cups.splice(std::next(insertion_point), extracted_cups);
        
        current_cup = circular_next(cups, current_cup);
    }

    current_cup = circular_next(cups, std::find(cups.begin(), cups.end(), 1));
    std::string result;
    while (*current_cup != 1) {
        result += std::to_string(*current_cup);
        current_cup = circular_next(cups, current_cup);
    }

    std::cout << "Result: " << result << std::endl;
}
