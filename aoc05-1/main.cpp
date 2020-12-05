#include <iostream>
#include <string>

int main() {
    std::istream &input = std::cin;
    int highest_id = 0;

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
        if (id > highest_id)
            highest_id = id;
    }

    std::cout << "Highest ID: " << highest_id << std::endl;
    return 0;
}
