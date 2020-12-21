#include <iostream>
#include <string>
#include <map>
#include <set>
#include <regex>

struct Food {
    std::set<std::string> ingredients;
    std::set<std::string> allergens;
};
static std::map<std::string,std::set<std::string>> allergens_possible_ingredients;
static std::map<std::string,std::string> ingredients_allergens;
static std::vector<Food> foods;

static void parse_foods(std::istream &input);
static void deduce_ingredients_allergens();
static bool ingredient_match_allergen(const std::string &ingr, const std::string &aller);
static int sum_foods_non_allergens();

int main () {
    std::istream &input = std::cin;

    parse_foods(input);
    deduce_ingredients_allergens();
    int sum = sum_foods_non_allergens();

    std::cout << "Non allergen ingredients sum in foods: " << sum << std::endl;
    return 0;
}

static void parse_foods(std::istream &input) {
    std::regex line_regex("^([[:alpha:] ]+) \\(contains ([[:alpha:] ,]+)\\)$");
    std::regex ingrs_sep_regex(" ");
    std::regex allers_sep_reges(", ");
    std::sregex_token_iterator tokens_end;

    std::string line;
    while (std::getline(input, line)) {
        std::smatch matchs;
        if (!std::regex_match(line, matchs, line_regex)) {
            std::cerr << "Invalid line: " << line << '\n';
            continue;
        }

        Food this_food;

        std::string ingredients_str = matchs[1];
        std::sregex_token_iterator ingrs_it(ingredients_str.begin(), ingredients_str.end(), ingrs_sep_regex, -1);
        while (ingrs_it != tokens_end) {
            this_food.ingredients.insert(*ingrs_it);
            ingredients_allergens[*ingrs_it] = "";
            ingrs_it++;
        }

        std::string allergens_str = matchs[2];
        std::sregex_token_iterator aller_it(allergens_str.begin(), allergens_str.end(), allers_sep_reges, -1);
        while (aller_it != tokens_end) {
            this_food.allergens.insert(*aller_it);
            for (auto &ingr : this_food.ingredients)
                allergens_possible_ingredients[*aller_it].insert(ingr);
            aller_it++;
        }

        foods.push_back(std::move(this_food));
    }
}

static void deduce_ingredients_allergens() {
    while (allergens_possible_ingredients.size() > 0) {
        auto aller_it = allergens_possible_ingredients.begin();
        while (aller_it != allergens_possible_ingredients.end()) {
            auto &aller = aller_it->first;
            auto &ingrs = aller_it->second;

            if (ingrs.size() == 1) {
                auto &ingr = *ingrs.begin();
                ingredients_allergens[ingr] = aller;
                aller_it = allergens_possible_ingredients.erase(aller_it);
                for (auto &[a, ingrs] : allergens_possible_ingredients)
                    ingrs.erase(ingr);
            }
            else {
                auto ingrs_it = ingrs.begin();
                while (ingrs_it != ingrs.end()) {
                    if (!ingredient_match_allergen(*ingrs_it, aller))
                        ingrs_it = ingrs.erase(ingrs_it);
                    else
                        ingrs_it++;
                }
                aller_it++;
            }
        }
    }
}

static bool ingredient_match_allergen(const std::string &ingr, const std::string &aller) {
    for (auto &food : foods) {
        if (food.allergens.count(aller) && !food.ingredients.count(ingr))
            return false;
    }
    return true;
}

static int sum_foods_non_allergens() {
    int sum = 0;
    for (auto &food : foods) {
        for (auto &ingr : food.ingredients) {
            if (ingredients_allergens[ingr] == "")
                sum++;
        }
    }
    return sum;
}