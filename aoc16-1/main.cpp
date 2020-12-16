#include <iostream>
#include <string>
#include <vector>
#include <regex>

struct num_range {
    int min, max;
};

struct ticket_field {
    std::string name;
    std::vector<num_range> values;
};

static std::vector<ticket_field> get_valid_fields_ranges(std::istream &input);
static std::vector<num_range> get_all_valid_num_ranges(std::vector<ticket_field> &fields);
static inline bool ranges_can_merge (num_range a, num_range b);
static inline num_range ranges_merge (num_range a, num_range b);
static void advance_lines(std::istream &input, unsigned int num_lines = 1);
static bool get_next_ticket (std::istream &input, std::vector<int> &ticket);
static std::vector<int> get_next_ticket (std::istream &input);
static int get_invalid_ticket_vals_sum(const std::vector<int> &ticket,
                const std::vector<num_range> &valid_ranges);

int main () {
    std::istream &input = std::cin;

    auto fields = get_valid_fields_ranges(input);
    auto valid_ranges = get_all_valid_num_ranges(fields);

    int sum = 0;

    advance_lines(input);
    std::vector<int> ticket = get_next_ticket(input);
    sum += get_invalid_ticket_vals_sum(ticket, valid_ranges);

    advance_lines(input, 2);
    while (get_next_ticket(input, ticket)) {
        sum += get_invalid_ticket_vals_sum(ticket, valid_ranges);
    }

    std::cout << "Sum invalid values: " << sum << std::endl;
    return 0;
}

static std::vector<ticket_field> get_valid_fields_ranges(std::istream &input) {
    std::regex field_regex("^([[:alpha:][:space:]]+): ([[:digit:]]+)-([[:digit:]]+) or ([[:digit:]]+)-([[:digit:]]+)$");
    static std::regex blank_regex("^[[:space:]]*$");

    std::vector<ticket_field> fields;

    std::string line;
    std::smatch matchs;
    while (std::getline(input, line) && !std::regex_match(line, blank_regex)) {
        if (!std::regex_match(line, matchs, field_regex)) {
            std::cerr << "Invalid field spec. line: " << line << '\n';
            continue;
        }
        num_range range1 {std::stoi(matchs[2]), std::stoi(matchs[3])};
        num_range range2 {std::stoi(matchs[4]), std::stoi(matchs[5])};
        fields.emplace_back(ticket_field{matchs[1], {std::move(range1), std::move(range2)}});
    }

    return fields;
}

static std::vector<num_range> get_all_valid_num_ranges(std::vector<ticket_field> &fields) {
    std::vector<num_range> valid_ranges;

    for (const auto &fld : fields) {
        for (const auto &fld_rng : fld.values) {
            bool merged = false;
            for (auto &glb_rng : valid_ranges) {
                if (ranges_can_merge(glb_rng, fld_rng)) {
                    glb_rng = ranges_merge(glb_rng, fld_rng);
                    merged = true;
                    break;
                }
            }
            if (!merged) {
                valid_ranges.push_back(fld_rng);
            }
        }
    }

    return valid_ranges;
}

static inline bool ranges_can_merge (num_range a, num_range b) {
    return (b.min <= a.max && b.max >= a.min) ||
           (a.min <= b.max && a.max >= a.min);
}

static inline num_range ranges_merge (num_range a, num_range b) {
    return {std::min(a.min, b.min), std::max(a.max, b.max)};
}

static bool get_next_ticket (std::istream &input, std::vector<int> &ticket) {
    std::string line, num;
    if (std::getline(input, line)) {
        ticket.clear();
        std::stringstream sstream(line);
        while (std::getline(sstream, num, ','))
            ticket.push_back(std::stoi(num));
        return true;
    }
    return false;
}

static std::vector<int> get_next_ticket (std::istream &input) {
    std::vector<int> ticket;
    if (!get_next_ticket(input, ticket))
        throw std::out_of_range("No next ticket found");
    return ticket;
}

static void advance_lines(std::istream &input, unsigned int num_lines) {
    while (num_lines > 0) {
        if (input.get() == '\n')
            num_lines--;
    }
}

static int get_invalid_ticket_vals_sum(const std::vector<int> &ticket,
                const std::vector<num_range> &valid_ranges) {
    int sum = 0;
    for (int val : ticket) {
        bool found = false;
        for (auto [min, max] : valid_ranges) {
            if (val >= min && val <= max) {
                found = true;
                break;
            }
        }
        if (!found)
            sum += val;
    }
    return sum;
}
