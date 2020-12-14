#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <cmath>

constexpr size_t BITMASK_BITS_NUM = 36;

class bitmask {
public:
    bitmask();
    bitmask(const std::string &mask36bits);
    bitmask &operator= (const std::string &mask36bits);
    size_t num_masks();
    uint64_t mask(uint64_t num, size_t mask_num);
private:
    void set_mask(const std::string &mask36bits);
    std::vector<int> float_positions;
    uint64_t m_or;
};

using mem_t = std::map<uint64_t,uint64_t>;

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
            uint64_t mem_addr = std::stoi(matchs[1]);
            uint64_t mem_val = std::stoi(matchs[2]);
            for (size_t i = 0; i < mask.num_masks(); i++)
                mem[mask.mask(mem_addr, i)] = mem_val;
        }
        else {
            std::cerr << "Invalid line: " << line << '\n';
        }
    }

    unsigned long long sum = 0;
    for (auto [addr, val] : mem)
        sum += val;

    std::cout << "Memory sum = " << sum << std::endl;
    return 0;
}

bitmask::bitmask() {
}

bitmask::bitmask(const std::string &str) {
    set_mask(str);
}

bitmask &bitmask::operator= (const std::string &str) {
    set_mask(str);
    return *this;
}

size_t bitmask::num_masks() {
    return std::pow(2, float_positions.size());
}

void bitmask::set_mask(const std::string &str) {
    if (str.length() != BITMASK_BITS_NUM)
        throw std::invalid_argument("Mask must be " + std::to_string(BITMASK_BITS_NUM) + " bits length");

    m_or  = 0;
    float_positions.clear();
    for (size_t i = 0; i < BITMASK_BITS_NUM; i++) {
        switch (str[BITMASK_BITS_NUM - 1 - i]) {
            case '0':
                break;
            case '1':
                m_or |= static_cast<uint64_t>(1) << i;
                break;
            case 'X':
                float_positions.push_back(i);
                break;
            default:
                throw std::invalid_argument("Invalid mask format: " + str);
                break;
        }
    }
}

uint64_t bitmask::mask(uint64_t num, size_t num_mask) {
    if (num_mask >= num_masks())
        std::out_of_range("Mask num. out of range: " + std::to_string(num_mask));

    uint64_t float_mask_and = ~static_cast<uint64_t>(0);
    uint64_t float_mask_or = 0;
    for (size_t i = 0; i < float_positions.size(); i++) {
        if ((num_mask >> i) & 1)
            float_mask_or |= static_cast<uint64_t>(1) << float_positions[i];
        else
            float_mask_and &= ~(static_cast<uint64_t>(1) << float_positions[i]);
    }
    return (num & float_mask_and) | float_mask_or | m_or;
}

