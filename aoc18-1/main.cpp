#include <iostream>
#include <string>

using iterator_t = std::string::const_iterator;
static long parse_expression_and_calculate(iterator_t &&begin, iterator_t end);
static long parse_expression_and_calculate(iterator_t &begin, iterator_t end);
static long parse_operand(iterator_t &begin, iterator_t end);
static inline iterator_t find_closing_parenthesis(iterator_t begin, iterator_t end);
static inline char parse_operator(iterator_t &begin, iterator_t end);
static inline long calculate(long operand_left, char optr, long operand_right);
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
    long result = parse_operand(begin, end);
    while (begin != end) {
        char optr = parse_operator(begin, end);
        long op_r = parse_operand(begin, end);
        result    = calculate(result, optr, op_r);
        begin = skip_spaces(begin, end);
    }
    return result;
}

static long parse_operand(iterator_t &begin, iterator_t end) {
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

static inline long calculate(long operand_left, char optr, long operand_right) {
    switch (optr) {
        case '+': return operand_left + operand_right;
        case '*': return operand_left * operand_right;
        default: throw std::invalid_argument(std::string("Invalid operator ") + optr);
    }
    return 0;
}

static inline iterator_t skip_spaces(iterator_t begin, iterator_t end) {
    while (begin != end && isblank(*begin))
        begin++;
    return begin;
}
