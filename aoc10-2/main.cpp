#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

struct jolts_adapter {
    int jolts;
    unsigned long combs_to_end; // num. of combinations from this adapter to the end
};


/* 
 * Explanation of the algorithm: 
 * - each adapter can connect next to other adapter with 3 or less jolts higher
 * - last element is always needed, because device is just 3 more jolts than it
 * - the possible combinations from one adapter to the end is the sum of the combinations
 *   from each of the other adapters it can connect next. Example:
 *      - adapter A can connect to adapters B and C
 *      - adapter B can connect to adapter C (and maybe others)
 *      - from adapter B to the end there are 40 possible combinations
 *      - from adapter C to the end there are 25 possible combinations
 *      - if we connect adapter A to B, there are 40 possible combinations to the end
 *      - if we connect adapter A to C, there are 25 possible combinations to the end
 *      - connection A-B-C is already considered into the 40 combinations of connection A-B
 *      - total combinations from A is the sum of combinations from A-B and A-C
 * - calculate possible combinations for each adapter from end to beginning
 * - total possible combinations are the combinations for first element (being first element
 *   the power supply, not the first adapter)
 */

int main () {
    std::istream &input = std::cin;
    std::vector<jolts_adapter> adapters({{0, 0}});

    int num;
    while (input >> num)
        adapters.push_back({num, 0});
    std::sort(adapters.begin(), adapters.end(), [](auto a, auto b) {return a.jolts < b.jolts;});

    adapters.back().combs_to_end = 1;
    for (ssize_t pos_curr = adapters.size() - 2; pos_curr >= 0; pos_curr--) {
        for (size_t pos_jump = pos_curr + 1; pos_jump < adapters.size(); pos_jump++) {
            if (adapters[pos_jump].jolts - adapters[pos_curr].jolts > 3)
                break;
            adapters[pos_curr].combs_to_end += adapters[pos_jump].combs_to_end;
        }
    }

    std::cout << "Combinations " << adapters[0].combs_to_end << std::endl;
    return 0;
}
