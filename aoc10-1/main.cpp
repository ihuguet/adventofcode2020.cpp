#include <iostream>
#include <set>

int main () {
    std::istream &input = std::cin;
    std::multiset<int> adapters;

    int jolts;
    while (input >> jolts)
        adapters.insert(jolts);
    
    int diff1_cnt = 0, diff3_cnt = 1; // diff3 at least 1 (adapter to device diff)
    int jolts_prev = 0;
    for (auto it = adapters.cbegin(); it != adapters.cend(); it++) {
        jolts = *it;
        if (jolts - jolts_prev == 1)
            diff1_cnt++;
        else if (jolts - jolts_prev == 3)
            diff3_cnt++;
        jolts_prev = jolts;
    }

    std::cout << "Diff 1 cnt = " << diff1_cnt << ", diff 3 cnt = " << diff3_cnt << '\n';
    std::cout << "diff1 * diff3 = " << (diff3_cnt * diff1_cnt) << std::endl;
    return 0;
}
