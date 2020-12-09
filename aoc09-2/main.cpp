#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using num_t = uint_least64_t;
using vec_t = std::vector<num_t>;
using vec_iter_t = std::vector<num_t>::const_iterator;

static std::pair<num_t,bool> findWrongNum(const vec_t &nums);
static std::pair<vec_iter_t,vec_iter_t> findContiguousRangeThatSums(const vec_t &nums, num_t tgt_sum);

int main () {
    std::istream &input = std::cin;
    vec_t nums;

    num_t num;
    while (input >> num)
        nums.push_back(num);

    auto [wrong_num, found] = findWrongNum(nums);
    if (!found) {
        std::cerr << "Wrong number not found" << std::endl;
        return 1;
    }
    std::cout << "Wrong number = " << wrong_num << '\n';

    auto [cbegin, cend] = findContiguousRangeThatSums(nums, wrong_num);
    if (cbegin == nums.cend()) {
        std::cerr << "No contiguous range of numbers fount that sums the value of the wrong number" << std::endl;
        return 1;
    }
    std::cout << "Contiguous range that sums " << wrong_num << ": [" << (cbegin - nums.cbegin()) << ", " << (cend - nums.cbegin()) << ")\n";

    auto [min_it, max_it] = std::minmax_element(cbegin, cend);
    std::cout << "min = " << *min_it << ", max = " << *max_it << ", min + max = " << (*min_it + *max_it) << std::endl;

    return 0;
}

static std::pair<num_t,bool> findWrongNum(const vec_t &nums) {
    for (auto it_num = nums.cbegin() + 25; it_num != nums.cend(); it_num++) {
        bool ok = false;
        for (auto it1 = it_num - 25; !ok && it1 != it_num; it1++) {
            for (auto it2 = it_num - 25; !ok && it2 != it_num; it2++) {
                if (it1 != it2 && *it1 + *it2 == *it_num) 
                    ok = true;
            }
        }
        if (!ok)
            return std::pair<num_t,bool>(*it_num, true);
    }
    return std::pair<num_t,bool>(0, false);
}

static std::pair<vec_iter_t,vec_iter_t> findContiguousRangeThatSums(const vec_t &nums, num_t tgt_sum) {
    for (auto it1 = nums.cbegin(); it1 != nums.cend(); it1++) {
        num_t sum = 0;
        for (auto it2 = it1; it2 != nums.cend(); it2++) {
            sum += *it2;
            if (sum == tgt_sum)
                return std::pair<vec_iter_t,vec_iter_t>(it1, std::next(it2));
            else if (sum > tgt_sum)
                break;
        }
    }
    return std::pair<vec_iter_t,vec_iter_t>(nums.cend(), nums.cend());
}
