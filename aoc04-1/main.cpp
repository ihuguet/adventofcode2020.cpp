#include <iostream>
#include <string>
#include <regex>
#include <set>

static bool is_valid_passport(const std::string &passport_data);

int main ()
{
    std::istream &input = std::cin;
    std::string passport_data;
    std::string line;
    unsigned int valid_passport_cnt = 0, passports_cnt = 0;

    std::regex blankregex("\\s*");

    while (std::getline(input, line)) {
        if (!std::regex_match(line, blankregex)) {
            passport_data += line;
            passport_data += ' ';
        }
        else {
            passports_cnt++;
            if (is_valid_passport(passport_data))
                valid_passport_cnt++;
            passport_data.clear();
        }
    }
    if (passport_data.length() > 0) {
        passports_cnt++;
        if (is_valid_passport(passport_data))
            valid_passport_cnt++;
    }

    std::cout << "Valid passports: " << valid_passport_cnt << "/" << passports_cnt   << std::endl;
    return 0;
}

static bool is_valid_passport(const std::string &passport_data) {
    static std::regex rexpr(R"((.+?):(.+?)\s)");
    static std::vector<std::string> reqFlds = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};

    auto regex_match_it = std::sregex_iterator(passport_data.cbegin(), passport_data.cend(), rexpr);
    std::set<std::string> flds;
    for (; regex_match_it != std::sregex_iterator(); regex_match_it++) {
        auto fld = (*regex_match_it)[1];
        auto [it, inserted] = flds.insert(fld);
        if (!inserted)
            std::cerr << "Already existed: " << fld << '\n';
    }

    for (auto reqFld : reqFlds) {
        if (!flds.count(reqFld))
            return false;
    }
    return true;
}
