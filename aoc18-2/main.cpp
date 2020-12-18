#include <iostream>
#include <string>

using iterator_t = std::string::const_iterator;
static long parse_expression_and_calculate(iterator_t &&begin, iterator_t end);
static long parse_expression_and_calculate(iterator_t &begin, iterator_t end);
static long parse_precedent_subexpression_and_calculate(iterator_t &begin, iterator_t end);
static inline long parse_operand(iterator_t &begin, iterator_t end);
static inline iterator_t find_closing_parenthesis(iterator_t begin, iterator_t end);
static inline char parse_operator(iterator_t &begin, iterator_t end);
static inline iterator_t skip_spaces(iterator_t begin, iterator_t end);

int main () {
    std::istream &input = std::cin;

    long sum = 0;
    std::string line;
    while (std::getline(input, line)) {
        long res = parse_expression_and_calculate(line.cbegin(), line.cend());
        std::cout << line << " = " << res << '\n';
        sum += res;
    }


    std::cout << "Sum = " << sum << std::endl;
    return 0;
}

static long parse_expression_and_calculate(iterator_t &&begin, iterator_t end) {
    return parse_expression_and_calculate(begin, end);
}

static long parse_expression_and_calculate(iterator_t &begin, iterator_t end) {
    long result = parse_precedent_subexpression_and_calculate(begin, end);
    begin = skip_spaces(begin, end);
    while (begin != end) {
        if (parse_operator(begin, end) != '*')
            throw std::string("Invalid operator");
        result *= parse_precedent_subexpression_and_calculate(begin, end);
        begin = skip_spaces(begin, end);
    }
    return result;
}

static long parse_precedent_subexpression_and_calculate(iterator_t &begin, iterator_t end) {
    long result = 0;
    skip_spaces(begin, end);
    while (begin != end) {
        result += parse_operand(begin, end);
        begin = skip_spaces(begin, end);
        if (begin == end || *begin != '+')
            break;
        begin++;
    }
    return result;
}

static inline long parse_operand(iterator_t &begin, iterator_t end) {
    begin = skip_spaces(begin, end);

    if (*begin == '(') {
        auto end_parenthesis = find_closing_parenthesis(begin, end);
        long res = parse_expression_and_calculate(++begin, std::prev(end_parenthesis));
        begin = end_parenthesis;
        return res;
    }

    std::string num_str;
    while (begin != end && isdigit(*begin))
        num_str += *(begin++);

    return std::stoi(num_str);
}

static inline iterator_t find_closing_parenthesis(iterator_t begin, iterator_t end) {
    if (begin == end || *begin != '(')
        return begin;
    
    int nesting_lvl = 0;
    do {
        if (*begin == '(')
            nesting_lvl++;
        else if (*begin == ')')
            nesting_lvl--;
        begin++;
    } while (begin != end && nesting_lvl > 0);

    return begin;
}

static inline char parse_operator(iterator_t &begin, iterator_t end) {
    begin = skip_spaces(begin, end);
    return *(begin++);
}

static inline iterator_t skip_spaces(iterator_t begin, iterator_t end) {
    while (begin != end && isblank(*begin))
        begin++;
    return begin;
}
