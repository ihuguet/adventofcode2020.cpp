#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>

using RoundDecks = std::pair<std::list<int>,std::list<int>>;
static std::list<int> parse_player_cards(std::istream &input);
static void play_game(std::list<int> &player1, std::list<int> &player2);
static bool decks_match_previous_round(const std::vector<RoundDecks> &prev_rounds,
                                       const std::list<int> &player1, const std::list<int> &player2);
static int sum_points(const std::list<int> &player_cards);

int main () {
    std::istream &input = std::cin;

    std::list<int> player1 = parse_player_cards(input);
    std::list<int> player2 = parse_player_cards(input);

    play_game(player1, player2);
    if (player1.size() > 0) {
        std::cout << "Winner: player 1 (" << sum_points(player1) << " points)" << std::endl;
        std::copy(player1.cbegin(), player1.cend(), std::ostream_iterator<int>(std::cout, " "));
    }
    else {
        std::cout << "Winner: player 2 (" << sum_points(player2) << " points)" << std::endl;
        std::copy(player2.cbegin(), player2.cend(), std::ostream_iterator<int>(std::cout, " "));
    }
    std::cout << std::endl;
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

static void play_game(std::list<int> &player1, std::list<int> &player2) {
    std::vector<RoundDecks> prev_rounds;

    while (player1.size() > 0 && player2.size() > 0) {
        if (decks_match_previous_round(prev_rounds, player1, player2)) {
            player2.clear();
            break;
        }
        else {
            prev_rounds.emplace_back(player1, player2);
        }

        int card1 = player1.front();
        int card2 = player2.front();
        player1.pop_front();
        player2.pop_front();

        bool winner1;
        if (player1.size() < static_cast<size_t>(card1) || player2.size() < static_cast<size_t>(card2)) {
            winner1 = card1 > card2;
        }
        else {
            std::list<int> player1_subgame, player2_subgame;
            auto it1 = player1.cbegin();
            for (int i = 0; i < card1; i++)
                player1_subgame.push_back(*it1++);
            auto it2 = player2.cbegin();
            for (int i = 0; i < card2; i++)
                player2_subgame.push_back(*it2++);

            play_game(player1_subgame, player2_subgame);
            winner1 = player1_subgame.size() > 0;
        }

        if (winner1) {
            player1.push_back(card1);
            player1.push_back(card2);
        }
        else {
            player2.push_back(card2);
            player2.push_back(card1);
        }
    }
}

static bool decks_match_previous_round(const std::vector<RoundDecks> &prev_rounds,
                                       const std::list<int> &player1, const std::list<int> &player2) {
    auto found = std::find_if(prev_rounds.cbegin(), prev_rounds.cend(), [&](const RoundDecks &decks) -> bool {
        return std::equal(decks.first.cbegin(), decks.first.cend(), player1.cbegin(), player1.cend())
            && std::equal(decks.second.cbegin(), decks.second.cend(), player2.cbegin(), player2.cend());
    });
    return found != prev_rounds.cend();
}

static int sum_points(const std::list<int> &cards) {
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
