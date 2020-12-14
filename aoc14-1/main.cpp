#include <iostream>
#include <string>
#include <regex>

constexpr size_t BITMASK_BITS_NUM = 36;

class bitmask {
public:
    bitmask();
    bitmask(const std::string &mask36bits);
    bitmask &operator= (const std::string &mask36bits);
    uint64_t mask(uint64_t num);
private:
    void set_mask(const std::string &mask36bits);
    uint64_t m_and, m_or;
};

using mem_t = std::map<size_t,unsigned long>;

int main () {
    std::istream &input = std::cin;
    std::regex mask_regex(R"(^mask = ([01X]{36})$)");
    std::regex mem_regex(R"(^mem\[(\d+)\] = (\d+)$)");

    bitmask mask;
    mem_t mem;

    std::string line;
    while (std::getline(input, line)) {
        std::smatch matchs;
        
        if (std::regex_match(line, matchs, mask_regex)) {
            mask = matchs.str(1);
        }
        else if (std::regex_match(line, matchs, mem_regex)) {
            size_t mem_addr = std::stoi(matchs[1]);
            unsigned long mem_val = std::stoi(matchs[2]);
            mem[mem_addr] = mask.mask(mem_val);
        }
        else {
            std::cerr << "Invalid line: " << line << '\n';
        }
    }

    unsigned long sum = 0;
    for (auto [addr, val] : mem)
        sum += val;

    std::cout << "Memory sum = " << sum << std::endl;
    return 0;
}

bitmask::bitmask() : m_and(~static_cast<uint64_t>(0)), m_or(0) {
}

bitmask::bitmask(const std::string &str) {
    set_mask(str);
}

bitmask &bitmask::operator= (const std::string &str) {
    set_mask(str);
    return *this;
}

void bitmask::set_mask(const std::string &str) {
    if (str.length() != BITMASK_BITS_NUM)
        throw std::invalid_argument("Mask must be " + std::to_string(BITMASK_BITS_NUM) + " bits length");

    m_and = ~static_cast<uint64_t>(0);
    m_or  = 0;
    for (size_t i = 0; i < BITMASK_BITS_NUM; i++) {
        switch (str[BITMASK_BITS_NUM - 1 - i]) {
            case '0': m_and &= ~(static_cast<uint64_t>(1) << i); break;
            case '1': m_or  |=   static_cast<uint64_t>(1) << i; break;
            case 'X': break;
            default:  throw std::invalid_argument("Invalid mask format: " + str); break;
        }
    }
}

uint64_t bitmask::mask(uint64_t num) {
    return (num & m_and) | m_or;
}

