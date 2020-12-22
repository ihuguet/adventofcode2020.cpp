#include <iostream>
#include <string>
#include <list>

static std::list<int> parse_player_cards(std::istream &input);
static int sum_points(std::list<int> player_cards);

int main () {
    std::istream &input = std::cin;

    std::list<int> player1 = parse_player_cards(input);
    std::list<int> player2 = parse_player_cards(input);

    while (player1.size() > 0 && player2.size() > 0) {
        if (player1.front() > player2.front()) {
            player1.push_back(player1.front());
            player1.push_back(player2.front());
        }
        else {
            player2.push_back(player2.front());
            player2.push_back(player1.front());
        }
        player1.pop_front();
        player2.pop_front();
    }

    if (player1.size() > 0)
        std::cout << "Winner: player 1 (" << sum_points(player1) << " points)" << std::endl;
    else
        std::cout << "Winner: player 2 (" << sum_points(player2) << " points)" << std::endl;
    return 0;
}

static std::list<int> parse_player_cards(std::istream &input) {
    std::list<int> player_cards;

    std::string line;
    std::getline(input, line); // discard player header
    while (std::getline(input, line) && line != "") {
        player_cards.push_back(std::stoi(line));
    }

    return player_cards;
}

static int sum_points(std::list<int> cards) {
    int points = 0;
    
    int points_mult = 1;
    auto it = cards.crbegin();
    while (it != cards.crend()) {
        points += *it * points_mult;
        points_mult++;
        it++;
    }

    return points;
}
