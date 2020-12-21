#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <array>

struct Tile {
    int num;
    bool sides[4][10];
    Tile(int num) : num(num) {}
};

enum TileSide {
    TOP = 0, RIGHT, BOTTOM, LEFT
};

static std::list<Tile> parse_tiles(std::istream &input);
static inline unsigned int calc_square_root(unsigned int num);
static void print_corner_tiles(std::list<Tile> &tiles);
static std::list<Tile>::const_iterator find_match(const Tile &tile, TileSide side, const std::list<Tile> &tiles);
static Tile get_transformed_tile (const Tile &orig, int rots_cws_cnt, bool flip);
static inline void copy_tile_side(const bool orig[10], bool dest[10], bool flip);
static inline bool match_tiles_sides(const Tile &a, TileSide side, const Tile &b);
static inline bool match_tiles_sides(const bool a[10], const bool b[10]);

int main () {
    std::istream &input = std::cin;

    auto tiles = parse_tiles(input);
    print_corner_tiles(tiles);

    return 0;
}

static std::list<Tile> parse_tiles(std::istream &input) {
    std::list<Tile> tiles_list;

    std::string line;
    while (std::getline(input, line)) {
        Tile tile {std::stoi(line.substr(5, line.length() - 6))};

        bool px[10][10];
        for (int i = 0; i < 10; i++) {
            std::getline(input, line);
            if (line.length() != 10) throw std::invalid_argument("Px row is not length = 10");
            for (size_t j = 0; j < line.length(); j++)
                px[i][j] = line[j] == '#';
        }
        std::getline(input, line); // blank line

        for (int i = 0; i < 10; i++)
            tile.sides[TileSide::TOP][i] = px[0][i];
        for (int i = 0; i < 10; i++)
            tile.sides[TileSide::RIGHT][i] = px[i][9];
        for (int i = 0; i < 10; i++)
            tile.sides[TileSide::BOTTOM][i] = px[9][9 - i];
        for (int i = 0; i < 10; i++)
            tile.sides[TileSide::LEFT][i] = px[9 - i][0];

        tiles_list.push_back(std::move(tile));
    }

    return tiles_list;
}

static inline unsigned int calc_square_root(unsigned int num) {
    size_t result = 1;
    while (result * result < num)
        result++;
    if (result * result != num)
        throw std::logic_error("Cannot calc square root of " + std::to_string(num));
    return result;
}

static void print_corner_tiles(std::list<Tile> &tiles) {
    std::array<TileSide,4> sides = {TileSide::TOP, TileSide::RIGHT, TileSide::BOTTOM, TileSide::LEFT};

    std::cout << "Corner tiles: \n";
    long mult = 1;
    for (auto it = tiles.begin(); it != tiles.end(); it++) {
        int sides_with_match = 0;
        for (size_t i = 0; i < sides.size(); i++) {
            if (find_match(*it, sides[i], tiles) != tiles.end())
                sides_with_match++;
        }
        if (sides_with_match < 3) {
            mult *= it->num;
            std::cout << it->num << " (" << sides_with_match << " sides match)\n";
        }
    }
    std::cout << "Mult corner IDs = " << mult << '\n';
}

static std::list<Tile>::const_iterator find_match(const Tile &tile, TileSide side, const std::list<Tile> &tiles) {
    auto it = tiles.begin();
    while (it != tiles.end()) {
        if (it->num != tile.num) {
            for (int transformation = 0; transformation < 8; transformation++) {
                int rotation = transformation % 4;
                bool flip = static_cast<bool>(transformation/4);
                Tile transformed_tile = get_transformed_tile(*it, rotation, flip);
                if (match_tiles_sides(tile, side, transformed_tile))
                    return it;
            }
        }
        it++;
    }
    return it;
}

static Tile get_transformed_tile (const Tile &orig, int rots_cws_cnt, bool flip) {
    Tile tile{orig.num};

    const bool *top, *right, *bottom, *left;
    switch (rots_cws_cnt) {
        case 0:
            top = orig.sides[TileSide::TOP]; right = orig.sides[TileSide::RIGHT];
            bottom = orig.sides[TileSide::BOTTOM]; left = orig.sides[TileSide::LEFT];
            break;
        case 1:
            top = orig.sides[TileSide::LEFT]; right = orig.sides[TileSide::TOP];
            bottom = orig.sides[TileSide::RIGHT]; left = orig.sides[TileSide::BOTTOM];
            break;
        case 2:
            top = orig.sides[TileSide::BOTTOM]; right = orig.sides[TileSide::LEFT];
            bottom = orig.sides[TileSide::TOP]; left = orig.sides[TileSide::RIGHT];
            break;
        case 3:
            top = orig.sides[TileSide::RIGHT]; right = orig.sides[TileSide::BOTTOM];
            bottom = orig.sides[TileSide::LEFT]; left = orig.sides[TileSide::TOP];
            break;
        default:
            throw std::invalid_argument("Rotation must be 0-3");
    }
    copy_tile_side(top, tile.sides[TileSide::TOP], flip);
    copy_tile_side(right, tile.sides[TileSide::RIGHT], flip);
    copy_tile_side(bottom, tile.sides[TileSide::BOTTOM], flip);
    copy_tile_side(left, tile.sides[TileSide::LEFT], flip);

    return tile;
}

static inline void copy_tile_side(const bool orig[10], bool dest[10], bool flip) {
    for (int i = 0; i < 10; i++) {
        dest[i] = orig[flip ? 9 - i : i];
    }
}

static inline bool match_tiles_sides(const Tile &a, TileSide side, const Tile &b) {
    TileSide b_side;
    switch (side) {
        case TileSide::TOP:    b_side = TileSide::BOTTOM; break;
        case TileSide::RIGHT:  b_side = TileSide::LEFT; break;
        case TileSide::BOTTOM: b_side = TileSide::TOP; break;
        case TileSide::LEFT:   b_side = TileSide::RIGHT; break;
        default: throw std::invalid_argument("Invalid side");
    }
    return match_tiles_sides(a.sides[side], b.sides[b_side]);
}

static inline bool match_tiles_sides(const bool a[10], const bool b[10]) {
    for (int i = 0; i < 10; i++) {
        if (a[i] != b[9-i])
            return false;
    }
    return true;
}