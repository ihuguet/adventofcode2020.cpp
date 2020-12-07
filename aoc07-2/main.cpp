#include <iostream>
#include <string>
#include <regex>
#include <list>
#include <map>
#include <memory>
#include <set>

class BagNode {
public:
    using const_iterator = std::list<std::pair<BagNode*,int>>::const_iterator;

    explicit BagNode(const std::string &color) noexcept
        : color(color) {
    }
    BagNode(BagNode &other) = delete;
    BagNode& operator=(const BagNode &other) = delete;
    BagNode(BagNode &&other) {
        copy(std::move(other));
    }
    BagNode& operator=(BagNode &&other) {
        return copy(std::move(other));
    }
    void add_bag(const BagNode &parent, int cnt) {
        children.push_back(std::make_pair(&const_cast<BagNode &>(parent), cnt));
    }
    std::string_view get_color() const noexcept {
        return std::string_view(color.data(), color.length());
    }
    const_iterator begin() const noexcept {
        return children.cbegin();
    }
    const_iterator end() const noexcept {
        return children.cend();
    }
private:
    BagNode &copy(BagNode &&other) {
        if (this != &other) {
            color = std::move(other.color);
            children = std::move(other.children);
        }
        return *this;
    }
    std::string color;
    std::list<std::pair<BagNode*,int>> children; // raw-pointers because shared_ptr can create circular refs.
                                  // all BagNode will be destroyed at the same time, so no problem with raw ptrs.
};

static std::map<std::string, BagNode> create_bags_graph (std::istream &input);
int cnt_contained_bags (const BagNode &node);

static std::regex pri_regex("([[:alpha:] ]+) bags contain((?: [[:digit:]]+ [[:alpha:] ]+ bags?[,.])+| no other bags\\.)");
static std::regex sec_regex("([[:digit:]]+) ([[:alpha:] ]+) bags?[,.]");

int main(int argc, char *argv[]) {
    if (argc > 2) {
        std::cerr << "Too many args\n";
        exit(1);
    }
    std::istream &input = std::cin;
    std::string color(argc == 2 ? argv[1] : "shiny gold");

    std::map<std::string, BagNode> bags = create_bags_graph(input);
    BagNode &node = bags.find(color)->second;
    int total_contained_bags = cnt_contained_bags(node);

    std::cout << "Total contained bags: " << total_contained_bags << std::endl;
    return 0;
}

static std::map<std::string, BagNode> create_bags_graph (std::istream &input) {
    std::map<std::string, BagNode> bags;
    
    std::string line;
    while (std::getline(input, line)) {
        std::smatch pri_matchs;
        if (!std::regex_match(line, pri_matchs, pri_regex)) {
            std::cerr << "Invalid  line: bad formatting \"" << line << "\"\n";
            continue;
        }

        std::string container_color = pri_matchs.str(1);
        auto it = bags.find(container_color);
        if (it == bags.end())
            it = bags.emplace(container_color, BagNode(container_color)).first;
        BagNode &container_node = it->second;

        std::sregex_iterator sec_matchs_it(pri_matchs[2].first, pri_matchs[2].second, sec_regex);
        std::sregex_iterator sec_matchs_end;
        while (sec_matchs_it != sec_matchs_end) {
            std::smatch sec_matchs = *sec_matchs_it;
            for (size_t i = 1; i < sec_matchs.size(); i+=2) {
                int contained_node_cnt = std::stoi(sec_matchs.str(i));
                std::string contained_color = sec_matchs.str(i + 1);
                if ((it = bags.find(contained_color)) == bags.end())
                    it = bags.emplace(contained_color, BagNode(contained_color)).first;
                BagNode &contained_node = it->second;
                
                container_node.add_bag(contained_node, contained_node_cnt);
            }
            sec_matchs_it++;
        }
    }

    return bags;
}

int cnt_contained_bags (const BagNode &node) {
    int total = 0;
    for (auto const &[child, cnt] : node) {
        total += cnt;
        total += cnt * cnt_contained_bags(*child);
    }
    return total;
}
