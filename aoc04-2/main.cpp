#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <functional>
#include <vector>

static bool read_next_passport (std::istream &input, std::string &passport_data);
static bool validate_passport(const std::string &passport_data);
static bool validate_year(const std::string &year, unsigned min, unsigned max);
static bool validate_height(const std::string &height);
static bool validate_hair(const std::string &color);
static bool validate_eyes(const std::string &color);
static bool validate_pid(const std::string &num);

using validate_fn_t = std::function<bool(const std::string &)>;
static std::map<std::string,validate_fn_t> req_flds = {
    {"byr", [](const std::string &val) {return validate_year(val, 1920, 2002);}},
    {"iyr", [](const std::string &val) {return validate_year(val, 2010, 2020);}},
    {"eyr", [](const std::string &val) {return validate_year(val, 2020, 2030);}},
    {"hgt", validate_height},
    {"hcl", validate_hair},
    {"ecl", validate_eyes},
    {"pid", validate_pid}
};

int main() {
    unsigned int valid_passport_cnt = 0, passports_cnt = 0;

    std::istream &input = std::cin;
    std::string passport_data;
    while (read_next_passport(input, passport_data)) {
        passports_cnt++;
        if (validate_passport(passport_data))
            valid_passport_cnt++;
    }

    std::cout << "Valid passports: " << valid_passport_cnt << "/" << passports_cnt   << std::endl;
    return 0;
}

static bool read_next_passport (std::istream &input, std::string &passport_data) {
    static std::regex blankregex("\\s*");

    bool data_found = false;
    passport_data.clear();

    std::string line;
    while (std::getline(input, line) && !std::regex_match(line, blankregex)) {
        passport_data += line;
        passport_data += ' ';
        data_found = true;
    }

    return data_found;
}

static bool validate_passport(const std::string &passport_data) {
    static std::regex rexpr(R"((.+?):(.+?)\s)");

    std::map<std::string,std::string> flds;
    auto matchs_it = std::sregex_iterator(passport_data.cbegin(), passport_data.cend(), rexpr);
    while (matchs_it != std::sregex_iterator()) {
        auto fld = (*matchs_it)[1].str();
        auto val = (*matchs_it)[2].str();
        auto [it, inserted] = flds.insert(std::pair(std::move(fld), std::move(val)));
        if (!inserted)
            std::cerr << "Already existed: " << (*matchs_it)[1].str() << '\n';
        matchs_it++;
    }

    for (auto [req_fld, validate_fn] : req_flds) {
        auto it = flds.find(req_fld);
        if (it == flds.end())
            return false;
        if (!validate_fn(it->second))
            return false;
    }
    return true;
}

static bool validate_year(const std::string &year, unsigned min, unsigned max){
    static std::regex validate_rexpr("^[[:digit:]]{4}$");

    if (!std::regex_match(year, validate_rexpr))
        return false;
    
    unsigned int iyear = std::stoi(year);
    return iyear >= min && iyear <= max;
}

static bool validate_height(const std::string &height) {
    static std::regex validate_rexpr("^([[:digit:]]+?)(cm|in)$");

    std::smatch match;
    if (!std::regex_match(height, match, validate_rexpr))
        return false;

    int iheight = std::stoi(match[1]);
    if (match[2] == "cm")
        return iheight >= 150 && iheight <= 193;
    else if (match[2] == "in")
        return iheight >= 59 && iheight <= 76;
    return false;
}

static bool validate_hair(const std::string &color) {
    static std::regex validate_rexpr("^#[0-9a-fA-F]{6}$");
    return std::regex_match(color, validate_rexpr);
}

static bool validate_eyes(const std::string &color) {
    static std::vector<std::string> valid_vals = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
    return std::find(valid_vals.cbegin(), valid_vals.cend(), color) != valid_vals.cend();
}

static bool validate_pid(const std::string &num) {
    static std::regex validate_rexpr("^[[:digit:]]{9}$");
    return std::regex_match(num, validate_rexpr);
}
