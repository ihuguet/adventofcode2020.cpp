#include <iostream>
#include <string>
#include <set>

/*
 * X axis -> horizontal
 * Y axis -> 45º CCWS from X
 * +1 East -> 1, 0
 * +1 West -> -1, 0
 * +1 NE -> 0, 1
 * +1 SW -> 0, -1
 * +1 NW -> -1, 1
 * +1 SE -> 1, -1
 */

static std::pair<int,int> calc_tile_pos(const std::string &tile_str);

int main () {
    std::istream &input = std::cin;

    std::set<std::pair<int,int>> black_tiles;

    std::string tile_str;
    while (std::getline(input, tile_str)) {
        auto xy = calc_tile_pos(tile_str);
        if (auto it = black_tiles.find(xy); it != black_tiles.end())
            black_tiles.erase(it);
        else
            black_tiles.insert(xy);
    }

    std::cout << "Number of black tiles: " << black_tiles.size() << std::endl;
    return 0;
}

static std::pair<int,int> calc_tile_pos(const std::string &tile_str) {
    std::pair<int,int> xy = {0, 0};
    for (size_t i = 0; i < tile_str.length(); i++) {
        switch (tile_str[i]) {
            case 'e':
                xy.first += 1;
                break;
            case 'w':
                xy.first -= 1;
                break;
            case 'n':
                if (tile_str[++i] == 'e') {
                    xy.second += 1;
                }
                else {
                    xy.first -= 1;
                    xy.second += 1;
                }
                break;
            case 's':
                if (tile_str[++i] == 'e') {
                    xy.first += 1;
                    xy.second -= 1;
                }
                else {
                    xy.second -= 1;
                }
                break;
            default:
                std::cerr << "Invalid char " << tile_str[i] << '\n';
                break;
        }
    }
    return xy;
}

