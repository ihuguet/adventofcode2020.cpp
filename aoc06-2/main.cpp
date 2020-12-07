#include <iostream>
#include <string>
#include <regex>

static constexpr bool validate_no_duplicate_rsps = true;

static size_t read_next_group (std::istream &input, std::string &group_str);

int main() {
    std::istream &input = std::cin;
    unsigned int yes_cnt = 0;

    size_t num_persons;
    std::string group_str;
    while (0 != (num_persons = read_next_group(input, group_str))) {
        std::sort(group_str.begin(), group_str.end());

        size_t pos = 0;
        while (pos < group_str.length()) {
            size_t pos_end = group_str.find_first_not_of(group_str[pos], pos);
            if (pos_end == std::string::npos)
                pos_end = group_str.length();
            if (pos_end - pos == num_persons)
                yes_cnt++;
            pos = pos_end;
        }
    }

    std::cout << "Answered yes: " << yes_cnt << std::endl;
    return 0;
}

static size_t read_next_group (std::istream &input, std::string &group_str) {
    static std::regex blankregex("^\\s*$");

    size_t num_persons = 0;
    group_str.clear();

    std::string line;
    while (std::getline(input, line) && !std::regex_match(line, blankregex)) {
        if constexpr (validate_no_duplicate_rsps) {
            std::sort(line.begin(), line.end());
            auto last = std::unique(line.begin(), line.end());
            line.erase(last, line.end());
        }

        group_str += line;
        num_persons++;
    }

    return num_persons;
}
