#include <iostream>
#include <string>
#include <regex>

static bool read_next_group (std::istream &input, std::string &group_str) ;

int main() {
    std::istream &input = std::cin;
    unsigned int yes_cnt = 0;

    std::string group_str;
    while (read_next_group(input, group_str)) {
        std::sort(group_str.begin(), group_str.end());
        auto last = std::unique(group_str.begin(), group_str.end());
        group_str.erase(last, group_str.end());
        yes_cnt += group_str.length();
    }

    std::cout << "Answered yes: " << yes_cnt << std::endl;
    return 0;
}

static bool read_next_group (std::istream &input, std::string &group_str) {
    static std::regex blankregex("^\\s*$");

    bool data_found = false;
    group_str.clear();

    std::string line;
    while (std::getline(input, line) && !std::regex_match(line, blankregex)) {
        group_str += line;
        data_found = true;
    }

    return data_found;
}
