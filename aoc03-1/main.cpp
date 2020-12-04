#include <iostream>
#include <string>

int main ()
{
    std::istream &input = std::cin;
    char ch;
    
    // get line length
    std::string line;
    std::getline(input, line);
    size_t row_length = line.length();

    // count trees
    int trees_cnt = 0;
    if (line[0] == '#')
        trees_cnt++;
    int pos = 3;
    while (input.good()) {
        input.ignore(pos, '\n');
        
        input.get(ch);
        if (ch == '#')
            trees_cnt++;
        
        input.ignore(row_length, '\n');
        if ((pos += 3) >= row_length)
            pos -= row_length;
    }

    std::cout << "Trees: " << trees_cnt << std::endl;
    return 0;
}
