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

using ticket = std::vector<int>;

static std::vector<ticket_field> get_valid_fields_ranges(std::istream &input);
static std::vector<num_range> get_all_valid_num_ranges(std::vector<ticket_field> &fields);
static inline bool ranges_can_merge (num_range a, num_range b);
static inline num_range ranges_merge (num_range a, num_range b);
static void advance_lines(std::istream &input, unsigned int num_lines = 1);
static bool get_next_ticket (std::istream &input, std::vector<int> &ticket);
static std::vector<int> get_next_ticket (std::istream &input);
static bool validate_ticket(const std::vector<int> &ticket, const std::vector<num_range> &valid_ranges);
static std::vector<ticket_field> deduce_fields_order(const std::vector<ticket> &tickets,
                const std::vector<ticket_field> &fields);
static int remove_fields_with_invalid_range_for_val(std::vector<ticket_field> &fields, int val);
static int remove_fields_with_names(std::vector<ticket_field> &fields,
                const std::vector<std::string> &names);

int main () {
    std::istream &input = std::cin;

    auto fields_unordered = get_valid_fields_ranges(input);
    auto valid_ranges = get_all_valid_num_ranges(fields_unordered);

    advance_lines(input);
    ticket ticket_mine = get_next_ticket(input);

    advance_lines(input, 2);
    std::vector<ticket> tickets_passengers;
    ticket ticket_tmp;
    while (get_next_ticket(input, ticket_tmp)) {
        if (validate_ticket(ticket_tmp, valid_ranges))
            tickets_passengers.push_back(std::move(ticket_tmp));
    }

    std::vector<ticket_field> fields_ordered = deduce_fields_order(tickets_passengers, fields_unordered);

    long mult = 1;
    for (size_t i = 0; i < fields_ordered.size(); i++) {
        if (fields_ordered[i].name.substr(0, 9) == "departure")
            mult *= ticket_mine[i];
    }

    std::cout << "Multiply departure values: " << mult << std::endl;
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

static bool validate_ticket(const std::vector<int> &ticket, const std::vector<num_range> &valid_ranges) {
    for (int val : ticket) {
        bool found = false;
        for (auto [min, max] : valid_ranges) {
            if (val >= min && val <= max) {
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }
    return true;
}

static std::vector<ticket_field> deduce_fields_order(const std::vector<ticket> &tickets,
                const std::vector<ticket_field> &fields) {
    const size_t num_ticket_fields = tickets[0].size();
    std::vector<std::vector<ticket_field>> fields_candidates(num_ticket_fields, fields);

    // discard candidates for which exist any ticket with a value out of range
    for (auto &ticket : tickets) {
        for (size_t i = 0; i < num_ticket_fields; i++)
            remove_fields_with_invalid_range_for_val(fields_candidates[i], ticket[i]);
    }

    // fields with only one candidate are confirmed, discard it for the rest, and repeat until end
    std::vector<ticket_field> fields_ordered(num_ticket_fields);
    std::vector<std::string> fields_to_remove;
    bool solved = false;
    while (!solved) {
        solved = true;
        bool action_done = false;
        for (size_t i = 0; i < num_ticket_fields; i++) {
            auto &cands = fields_candidates[i];
            if (cands.size() == 1) {
                fields_to_remove.push_back(cands.front().name);
                fields_ordered[i] = std::move(cands.front());
                cands.pop_back();
                action_done = true;
            }
            else if (cands.size() > 1) {
                int rm_cnt = remove_fields_with_names(cands, fields_to_remove);
                solved = false;
                if (rm_cnt > 0)
                    action_done = true;
            }
        }
        if (!action_done) // stuck
            throw std::logic_error("Fields cannot be deduced");
    }

    return fields_ordered;
}

static int remove_fields_with_invalid_range_for_val(std::vector<ticket_field> &fields, int val) {
    auto it = std::remove_if(fields.begin(), fields.end(), [val](auto &fld) {
        for (auto [min, max] : fld.values) {
            if (val >= min && val <=max)
                return false;
        }
        return true;
    });
    int removed_cnt = std::distance(it, fields.end());
    fields.erase(it, fields.end());
    return removed_cnt;
}

static int remove_fields_with_names(std::vector<ticket_field> &fields,
                const std::vector<std::string> &names) {
    auto it = std::remove_if(fields.begin(), fields.end(), [&names](auto &fld) {
        return std::find(names.begin(), names.end(), fld.name) != names.end();
    });
    int removed_cnt = std::distance(it, fields.end());
    fields.erase(it, fields.end());
    return removed_cnt;
}
