#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

constexpr int NUMS_TO_COUNT = 30'000'000;

int main () {
    std::vector<int> nums = {0,5,4,1,10,14,7};
    std::vector<int> nums_last_pos(NUMS_TO_COUNT, -1);
    for (size_t i = 0; i < nums.size() - 1; i++)
        nums_last_pos[nums[i]] = i;

    int last_num = nums.back();
    for (int nums_cnt = nums.size() - 1; nums_cnt < NUMS_TO_COUNT - 1; nums_cnt++) {
        int last_num_pos_diff = nums_last_pos[last_num] != -1 ? nums_cnt - nums_last_pos[last_num] : 0;
        nums_last_pos[last_num] = nums_cnt;
        last_num = last_num_pos_diff;
    }

    std::cout << NUMS_TO_COUNT << "th num: " << last_num << std::endl;
    return 0;
}
