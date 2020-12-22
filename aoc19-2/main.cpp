#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <regex>

using Rule = std::vector<std::string>;
using RulesList = std::map<int,Rule>;
using UnsolvedRule = std::vector<std::pair<int,int>>;
using UnsolvedRulesList = std::map<int,UnsolvedRule>;

static void create_rules_list (std::istream &input);
static void parse_rules (std::istream &input, UnsolvedRulesList &unsolved_rules);
static bool can_solve_rule(const UnsolvedRule &unslvd_rule);
static Rule solve_rule(const UnsolvedRule &unslvd_rule);
static int cnt_valid_strings (std::istream &input);
static bool match_rule0_8 (const std::string_view str);
static bool match_rule0_11 (const std::string_view str, int recurse_cnt = 0);
static bool match_rule11_31(const std::string_view str, int recurse_cnt);
static inline bool str_beg_match_rule_str(const std::string_view str, const std::string_view rule_str);

static RulesList rules;

int main () {
    std::istream &input = std::cin;

    create_rules_list(input);
    int cnt = cnt_valid_strings(input);

    std::cout << "count = " << cnt << std::endl;
    return 0;
}

static void create_rules_list (std::istream &input) {
    UnsolvedRulesList unsolved_rules;

    parse_rules(input, unsolved_rules);

    while (unsolved_rules.size() > 0) {
        auto it = unsolved_rules.begin();
        while (it != unsolved_rules.end()) {
            int rule_num = it->first;
            auto &rule_content = it->second;
            if (can_solve_rule(rule_content)) {
                rules[rule_num] = solve_rule(rule_content);
                it = unsolved_rules.erase(it);
            }
            else {
                it++;
            }
        }
    }
}

static void parse_rules (std::istream &input, UnsolvedRulesList &unsolved_rules) {
    static std::regex slvd_rule_regx("^([[:digit:]]+): \"([[:alpha:]]+)\"$");
    static std::regex unslvd_1rule_regx("^([[:digit:]]+): ([[:digit:]]+) ?([[:digit:]]+)?$");
    static std::regex unslvd_2rules_regx("^([[:digit:]]+): ([[:digit:]]+) ?([[:digit:]]+)? \\| ([[:digit:]]+) ?([[:digit:]]+)?$");

    std::string line;
    while (std::getline(input, line) && line != "") {
        std::smatch matchs;
        if (std::regex_match(line, matchs, slvd_rule_regx)) {
            rules[std::stoi(matchs[1])] = std::vector<std::string>({matchs[2]});
        }
        else if (std::regex_match(line, matchs, unslvd_1rule_regx)) {
            int second = matchs[3] != "" ? std::stoi(matchs[3]) : -1;
            unsolved_rules[std::stoi(matchs[1])] = std::vector({
                std::pair<int,int>({std::stoi(matchs[2]), second})
            });
        }
        else if (std::regex_match(line, matchs, unslvd_2rules_regx)) {
            int second1 = matchs[3] != "" ? std::stoi(matchs[3]) : -1;
            int second2 = matchs[5] != "" ? std::stoi(matchs[5]) : -1;
            unsolved_rules[std::stoi(matchs[1])] = std::vector({
                std::pair<int,int>({std::stoi(matchs[2]), second1}),
                std::pair<int,int>({std::stoi(matchs[4]), second2})
            });
        }
        else {
            std::cerr << "Invalid line: " << line << '\n';
        }
    }

    // erase special rules
    unsolved_rules.erase(0);
    unsolved_rules.erase(8);
    unsolved_rules.erase(11);
}

static bool can_solve_rule(const UnsolvedRule &unslvd_rule) {
    for (auto &rule_option : unslvd_rule) {
        if (!rules.count(rule_option.first) ||
            (rule_option.second != -1 && !rules.count(rule_option.second)))
            return false;
    }
    return true;
}

static Rule solve_rule(const UnsolvedRule &unslvd_rule) {
    Rule rule;
    for (auto &rule_option : unslvd_rule) {
        for (const std::string &substr1 : rules.at(rule_option.first)) {
            if (rule_option.second != -1) {
                for (const std::string &substr2 : rules.at(rule_option.second)) {
                    rule.push_back(substr1 + substr2);
                }
            }
            else {
                rule.push_back(substr1);
            }
        }
    }
    return rule;
}

static int cnt_valid_strings (std::istream &input) {
    int cnt = 0;
    std::string line;
    while (std::getline(input, line)) {
        if (match_rule0_8(std::string_view(line.data(), line.length()))) {
            std::cout << line << "\n";
            cnt++;
        }
    }
    return cnt;
}

static bool match_rule0_8 (const std::string_view str) {
    // first option of the rule (42)
    for (auto &str_compare : rules[42]) {
        if (!str_beg_match_rule_str(str, str_compare))
            continue;
        if (match_rule0_11(str.substr(str_compare.length())))
            return true;
    }

    // second option of the rule, the recursing one (42 8)
    for (auto &str_compare : rules[42]) {
        if (!str_beg_match_rule_str(str, str_compare))
            continue;
        if (match_rule0_8(str.substr(str_compare.length())))
            return true;
    }

    return false;    
}

static bool match_rule0_11 (const std::string_view str, int recurse_cnt) {
    static Rule rule_42_31 = solve_rule({{42, 31}});

    // first option of the rule (42 31)
    for (auto &str_compare : rule_42_31) {
        if (!str_beg_match_rule_str(str, str_compare))
            continue;
        if (match_rule11_31(str.substr(str_compare.length()), recurse_cnt)) // 3rd num. of 2nd option (42 11 31), must match loop_cnt times if looping
            return true;
    }

    // second option of the rule, the recursing one (42 11 31)
    for (auto &str_compare : rules[42]) {
        if (!str_beg_match_rule_str(str, str_compare))
            continue;
        if (match_rule0_11(str.substr(str_compare.length()), recurse_cnt + 1))
            return true;
    }

    return false;
}

static bool match_rule11_31(const std::string_view str, int recurse_cnt) {
    if (recurse_cnt == 0)
        return str == "";
    for (auto &str_compare : rules[31]) {
        if (!str_beg_match_rule_str(str, str_compare))
            continue;
        if (match_rule11_31(str.substr(str_compare.length()), recurse_cnt -1))
            return true;
    }
    return false;
}

static inline bool str_beg_match_rule_str(const std::string_view str, const std::string_view rule_str) {
    if (rule_str.length() > str.length())
        return false;
    return str.compare(0, rule_str.length(), rule_str) == 0;
}
