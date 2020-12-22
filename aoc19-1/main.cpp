#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>

using Rule = std::vector<std::string>;
using RulesList = std::map<int,Rule>;
using UnsolvedRule = std::vector<std::pair<int,int>>;
using UnsolvedRulesList = std::map<int,UnsolvedRule>;

static RulesList create_rules_list (std::istream &input);
static void parse_rules (std::istream &input, RulesList &rules, UnsolvedRulesList &unsolved_rules);
static bool can_solve_rule(UnsolvedRule &unslvd_rule, const RulesList &rules);
static Rule solve_rule(UnsolvedRule &unslvd_rule, const RulesList &rules);
static int cnt_valid_strings (std::istream &input, Rule &rules);


int main () {
    std::istream &input = std::cin;

    auto rules = create_rules_list(input);
    std::sort(rules[0].begin(), rules[0].end());
    int cnt = cnt_valid_strings(input, rules[0]);

    std::cout << "count = " << cnt << std::endl;
    return 0;
}

static RulesList create_rules_list (std::istream &input) {
    RulesList rules;
    UnsolvedRulesList unsolved_rules;

    parse_rules(input, rules, unsolved_rules);

    while (unsolved_rules.size() > 0) {
        auto it = unsolved_rules.begin();
        while (it != unsolved_rules.end()) {
            int rule_num = it->first;
            auto &rule_content = it->second;
            if (can_solve_rule(rule_content, rules)) {
                rules[rule_num] = solve_rule(rule_content, rules);
                it = unsolved_rules.erase(it);
            }
            else {
                it++;
            }
        }
    }

    return rules;
}

static void parse_rules (std::istream &input, RulesList &rules, UnsolvedRulesList &unsolved_rules) {
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
}

static bool can_solve_rule(UnsolvedRule &unslvd_rule, const RulesList &rules) {
    for (auto &rule_option : unslvd_rule) {
        if (!rules.count(rule_option.first) ||
            (rule_option.second != -1 && !rules.count(rule_option.second)))
            return false;
    }
    return true;
}

static Rule solve_rule(UnsolvedRule &unslvd_rule, const RulesList &rules) {
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

static int cnt_valid_strings (std::istream &input, Rule &rules) {
    int cnt = 0;
    std::string line;
    while (std::getline(input, line)) {
        if (std::binary_search(rules.cbegin(), rules.cend(), line))
            cnt++;
    }
    return cnt;
}
