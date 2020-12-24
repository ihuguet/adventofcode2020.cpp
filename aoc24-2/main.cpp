#include <iostream>
#include <string>
#include <map>
#include <algorithm>

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

constexpr int NUM_ROUNDS = 100;

enum class Color {BLACK, WHITE};
struct Coords {int x, y;};
using floor_t = std::map<Coords,Color>;

static Coords calc_tile_pos(const std::string &tile_str);
static void set_tile(floor_t &tiles, Coords coords, Color color);
static void toggle_tile(floor_t &tiles, Coords coords);
static int count_adjacents_black(floor_t &tiles, Coords coords);

static std::array<std::string,6> dirs = {"e", "se", "sw", "w", "nw", "ne"};

const Coords operator+(Coords a, Coords b) {
    return {a.x + b.x, a.y + b.y};
}

bool operator<(Coords a, Coords b) {
    return a.x != b.x ? a.x < b.x : a.y < b.y;
}

int main () {
    std::istream &input = std::cin;

    floor_t tiles;

    std::string tile_str;
    while (std::getline(input, tile_str)) {
        auto xy = calc_tile_pos(tile_str);
        if (!tiles.count(xy))
            set_tile(tiles, xy, Color::BLACK);
        else
            toggle_tile(tiles, xy);
    }

    for (int i = 0; i < NUM_ROUNDS; i++) {
        floor_t tiles_new;
        for (auto [xy, color] : tiles) {
            int adjacents_black = count_adjacents_black(tiles, xy);
            if (color == Color::BLACK) {
                if (adjacents_black == 1 || adjacents_black == 2)
                    set_tile(tiles_new, xy, Color::BLACK);
            }
            else {
                if (adjacents_black == 2)
                    set_tile(tiles_new, xy, Color::BLACK);
            }
        }
        tiles = std::move(tiles_new);
    }

    int count = std::count_if(tiles.cbegin(), tiles.cend(), [](auto e) {
        return e.second == Color::BLACK;
    });
    std::cout << "Number of black tiles: " << count << std::endl;
    return 0;
}

static Coords calc_tile_pos(const std::string &tile_str) {
    Coords xy = {0, 0};
    for (size_t i = 0; i < tile_str.length(); i++) {
        switch (tile_str[i]) {
            case 'e':
                xy = xy + Coords{1, 0};
                break;
            case 'w':
                xy = xy + Coords{-1, 0};
                break;
            case 'n':
                if (tile_str[++i] == 'e')
                    xy = xy + Coords{0, 1};
                else
                    xy = xy + Coords{-1, 1};
                break;
            case 's':
                if (tile_str[++i] == 'e')
                    xy = xy + Coords{1, -1};
                else
                    xy = xy + Coords{0, -1};
                break;
            default:
                std::cerr << "Invalid char " << tile_str[i] << '\n';
                break;
        }
    }
    return xy;
}

static void set_tile(floor_t &tiles, Coords coords, Color color) {
    tiles[coords] = color;
    for (auto &dir : dirs) {
        Coords adjacent_coords = coords + calc_tile_pos(dir);
        if (!tiles.count(adjacent_coords))
            tiles[adjacent_coords] = Color::WHITE;
    }
}

static void toggle_tile(floor_t &tiles, Coords coords) {
    set_tile(tiles, coords, tiles[coords] == Color::WHITE ? Color::BLACK : Color::WHITE);
}

static int count_adjacents_black(floor_t &tiles, Coords coords) {
    int cnt = 0;
    for (auto &dir : dirs) {
        Coords adjacent_coords = coords + calc_tile_pos(dir);
        if (tiles.count(adjacent_coords) && tiles[adjacent_coords] == Color::BLACK)
            cnt++;
    }
    return cnt;
}
