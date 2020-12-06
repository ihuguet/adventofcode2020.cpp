#include <iostream>
#include <string>
#include <regex>

static int read_next_group (std::istream &input, std::string &group_str);

int main() {
    std::istream &input = std::cin;
    unsigned int yes_cnt = 0;

    int num_persons;
    std::string group_str;
    while (0 != (num_persons = read_next_group(input, group_str))) {
        std::sort(group_str.begin(), group_str.end());

        auto ch_it = group_str.cbegin();
        while (ch_it != group_str.cend()) {
            auto ch_cnt = std::count(ch_it, group_str.cend(), *ch_it);
            if (ch_cnt == num_persons)
                yes_cnt++;
            ch_it += ch_cnt;
        }
    }

    std::cout << "Answered yes: " << yes_cnt << std::endl;
    return 0;
}

static int read_next_group (std::istream &input, std::string &group_str) {
    static std::regex blankregex("^\\s*$");

    int num_persons = 0;
    group_str.clear();

    std::string line;
    while (std::getline(input, line) && !std::regex_match(line, blankregex)) {
        // avoid possible duplication in 1 person answer
        std::sort(line.begin(), line.end());
        auto last = std::unique(line.begin(), line.end());
        line.erase(last, line.end());

        // add line to group string
        group_str += line;
        num_persons++;
    }

    return num_persons;
}
