#include <iostream>
#include <vector>
#include <algorithm>

constexpr int NUMS_TO_COUNT = 2020;

int main () {
    std::vector<int> nums = {0,5,4,1,10,14,7};
    nums.reserve(NUMS_TO_COUNT);

    while (nums.size() < NUMS_TO_COUNT) {
        int last_num = nums.back();
        auto it = std::find(std::next(nums.crbegin()), nums.crend(), last_num);
        int next_num = it != nums.crend() ? std::distance(nums.crbegin(), it) : 0;
        nums.push_back(next_num);
    }

    std::cout << NUMS_TO_COUNT << "th num: " << nums.back() << std::endl;
    return 0;
}
