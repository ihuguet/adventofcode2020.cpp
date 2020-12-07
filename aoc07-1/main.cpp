#include <iostream>
#include <string>
#include <regex>
#include <list>
#include <map>
#include <memory>
#include <set>

class BagNode {
public:
    using const_iterator = std::list<BagNode*>::const_iterator;

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
    void add_container(const BagNode &parent) {
        parents.push_back(&const_cast<BagNode &>(parent));
    }
    std::string_view get_color() const noexcept {
        return std::string_view(color.data(), color.length());
    }
    const_iterator begin() const noexcept {
        return parents.cbegin();
    }
    const_iterator end() const noexcept {
        return parents.cend();
    }
private:
    BagNode &copy(BagNode &&other) {
        if (this != &other) {
            color = std::move(other.color);
            parents = std::move(other.parents);
        }
        return *this;
    }
    std::string color;
    std::list<BagNode*> parents; // raw-pointers because shared_ptr can create circular refs.
                                 // all BagNode will be destroyed at the same time, so no problem with raw ptrs.
};

static std::map<std::string, BagNode> create_bags_graph (std::istream &input);
static std::set<std::string> get_containers_colors_all (const BagNode &node);

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
    std::set<std::string> parents_colors = get_containers_colors_all(node);

    std::cout << "Possible container bags for \"" << color << "\" bag: ";
    for (const auto &parent_color : parents_colors)
        std::cout << parent_color << ", ";
    std::cout << "\nTotal: " << parents_colors.size() << std::endl;
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
                std::string contained_color = sec_matchs.str(i + 1);
                if ((it = bags.find(contained_color)) == bags.end())
                    it = bags.emplace(contained_color, BagNode(contained_color)).first;
                BagNode &contained_node = it->second;
                
                contained_node.add_container(container_node);
            }
            sec_matchs_it++;
        }
    }

    return bags;
}

static void get_containers_colors_all(const BagNode &node, std::set<std::string> &out) {
    for (auto const &parent : node) {
        out.insert(std::string(parent->get_color()));
        get_containers_colors_all(*parent, out);
    }
}

static std::set<std::string> get_containers_colors_all (const BagNode &node) {
    std::set<std::string> containers_colors;
    get_containers_colors_all(node, containers_colors);
    return containers_colors;
}
