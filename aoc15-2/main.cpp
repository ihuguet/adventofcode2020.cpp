#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

constexpr int NUMS_TO_COUNT = 30'000'000;

int main () {
    std::vector<int> nums = {0,5,4,1,10,14,7};
    std::map<int,int> nums_last_turn;
    for (size_t i = 0; i < nums.size() - 1; i++)
        nums_last_turn[nums[i]] = i;

    int last_num = nums.back();
    for (int nums_cnt = nums.size() - 1; nums_cnt < NUMS_TO_COUNT - 1; nums_cnt++) {
        auto it = nums_last_turn.find(last_num);
        int num_prev_pos = it != nums_last_turn.end() ? nums_cnt - it->second : 0;
        nums_last_turn[last_num] = nums_cnt;
        last_num = num_prev_pos;
    }

    std::cout << NUMS_TO_COUNT << "th num: " << last_num << std::endl;
    return 0;
}
