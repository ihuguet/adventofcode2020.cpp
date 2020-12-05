#include <iostream>
#include <string>
#include <set>

int main() {
    std::istream &input = std::cin;
    std::set<int> ids;

    std::string line;
    while (std::getline(input, line)) {
        int row_start = 0, row_end = 128; // end excluded
        int col_start = 0, col_end = 8;   // end excluded

        for (size_t i = 0; i < 7; i++) {
            if (line[i] == 'F')
                row_end = (row_start + row_end) / 2;
            else if (line[i] == 'B')
                row_start = (row_start + row_end) / 2;
            else
                std::cerr << "Invalid line: " << line << '\n';
        }
        for (size_t i = 7; i < 10; i++) {
            if (line[i] == 'L')
                col_end = (col_start + col_end) / 2;
            else if (line[i] == 'R')
                col_start = (col_start + col_end) / 2;
            else
                std::cerr << "Invalid line: " << line << '\n';
        }

        int id = row_start * 8 + col_start;
        ids.insert(id);
    }

    auto it = ids.cbegin();
    int id = *it; // we're not in the front, so we directly discard firsts empty seats
    while (it != ids.cend()) { // we're not in the back, so we directly discard lasts empty seats
        if (id != *it) {
            std::cout << "Possible seat ID: " << id << std::endl;
            id++;
        }
        else {
            id++;
            it++;
        }
    }

    return 0;
}
