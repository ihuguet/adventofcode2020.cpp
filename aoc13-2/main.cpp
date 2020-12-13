#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using u64f = uint_fast64_t;
using vec_req = std::vector<std::pair<u64f,u64f>>;
static vec_req get_bus_requirements(std::istream &input);
static void sort_by_bus_line_desc(vec_req &reqs);
static void set_offsets_from(vec_req &reqs, std::pair<u64f,u64f> &bus_req);
static u64f gcd(u64f a, u64f b);
static u64f lcm(u64f a, u64f b);

int main () {
    auto reqs = get_bus_requirements(std::cin);
    u64f first_bus_line = reqs[0].first;

    sort_by_bus_line_desc(reqs);
    u64f higher_bus_line = reqs[0].first;
    set_offsets_from(reqs, reqs[0]);

    u64f time = 0;
    u64f time_inc = higher_bus_line;
    for (size_t next = 1; next < reqs.size(); next++) {
        auto [bus_line, time_offset] = reqs[next];

        // find next time that satisfy the rules for range [0, next]
        while ((time + time_offset) % bus_line != 0)
            time += time_inc;
        
        // set new time_inc: least common multiple is the repetition period
        time_inc = lcm(time_inc, bus_line);
    }
    
    for (auto [bus_line, time_offset] : reqs) {
        if (bus_line == first_bus_line) {
            time += time_offset;
            break;
        }
    }
 
    std::cout << "Timestamp: " << time << std::endl;
    return 0;
}

static vec_req get_bus_requirements(std::istream &input) {
    vec_req reqs;
    int mins_cnt = 0;

    std::string str;
    std::getline(input, str);
    while (std::getline(input, str, ',')) {
        if (str != "x") 
            reqs.emplace_back(std::stoi(str), mins_cnt);
        mins_cnt++;
    }

    return reqs;
}

static void sort_by_bus_line_desc(vec_req &reqs) {
    std::sort(reqs.begin(), reqs.end(), [](auto a, auto b) {return a.first > b.first;});
}

static void set_offsets_from(vec_req &reqs, std::pair<u64f,u64f> &bus_req) {
    u64f ref_time_offset = bus_req.second;
    for (auto &[bus_line, time_offset] : reqs)
        time_offset -= ref_time_offset;
}

// Recursive function to return gcd of a and b 
static u64f gcd(u64f a, u64f b) {
  if (b == 0)
    return a;
  return gcd(b, a % b);
}
 
// Function to return LCM of two numbers 
static u64f lcm(u64f a, u64f b) {
    return (a / gcd(a, b)) * b;
}