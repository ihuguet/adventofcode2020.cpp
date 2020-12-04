#include <iostream>
#include <fstream>
#include <string>
#include <array>

int count_trees (std::istream &input, int slope_right, int slope_down)
{
    char ch;
    
    // get line length
    std::string line;
    std::getline(input, line);
    size_t row_length = line.length();

    // count trees
    int trees_cnt = 0;
    if (line[0] == '#')
        trees_cnt++;
    int pos = 0;
    int down_cnt = 0;
    while (input.good()) {
        if (++down_cnt < slope_down) {
            input.ignore(row_length + 1, '\n');
            continue;
        }
        
        down_cnt = 0;
        if ((pos += slope_right) >= row_length)
            pos -= row_length;
        input.ignore(pos, '\n');
        
        input.get(ch);
        if (ch == '#')
            trees_cnt++;
        
        input.ignore(row_length + 1, '\n');
    }
    
    return trees_cnt;
}

#define P(f,s)  std::make_pair((f), (s))

int main (int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: main input_file_path" << std::endl;
        return 1;
    }
    
    std::fstream input(argv[1]);
    if (!input) {
        std::cout << "Error openning file '" << argv[1] << "'" << std::endl;
        return 1;
    }
    
    std::array<std::pair<int,int>, 5> slopes = {P(1, 1), P(3, 1), P(5, 1), P(7, 1), P(1, 2)};
    
    long result = 1;
    for (auto slope : slopes) {
        input.clear();
        input.seekg(0);
        int trees = count_trees(input, slope.first, slope.second);
        std::cout << "Slope " << slope.first << ", " << slope.second << "; trees " << trees << '\n';
        result *= trees;
    }
    std::cout << "Result: " << result << std::endl;
    return 0;
}
