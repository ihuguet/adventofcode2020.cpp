#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <array>

struct Tile {
    int num;
    bool sides[4][10];
    bool pixels[10][10];
    Tile(int num) : num(num) {}
};

enum TileSide {
    TOP = 0, RIGHT, BOTTOM, LEFT
};

struct TileMatch {
    std::list<Tile>::const_iterator tile_it;
    int rotation;
    bool flip;
};

static std::list<Tile> parse_tiles(std::istream &input);
static std::vector<std::vector<Tile>> arrange_img(int size_side, std::list<Tile> &tiles);
static void find_top_left_tile_and_place(std::vector<std::vector<Tile>> &img_tiles, std::list<Tile> &tiles);
static std::list<std::list<Tile>::iterator> find_corner_tiles(std::list<Tile> &tiles);
static void find_tile_and_place(std::vector<std::vector<Tile>> &img_tiles, int i, int j, std::list<Tile> &tiles);
static TileMatch find_match(const Tile &tile, TileSide side, const std::list<Tile> &tiles);
static Tile get_transformed_tile (const Tile &orig, int rots_cws_cnt, bool flip);
static std::vector<std::vector<bool>> merge_tiles (std::vector<std::vector<Tile>> &img_tiles);
static int count_monsters (const std::vector<std::vector<bool>> &pixels, int rot_cnt_cws, bool flip);
static inline void copy_tile_side(const bool orig[10], bool dest[10], bool flip);
static inline bool match_tiles_sides(const Tile &a, TileSide side, const Tile &b);
static inline bool match_tiles_sides(const bool a[10], const bool b[10]);
static inline bool is_corner_pos(int i, int j, int side_size);
static inline bool is_border_pos(int i, int j, int side_size);
static inline unsigned int calc_square_root(unsigned int num);

int main () {
    std::istream &input = std::cin;

    auto tiles = parse_tiles(input);
    size_t img_side_size = calc_square_root(tiles.size());
    auto img_tiles = arrange_img(img_side_size, tiles);
    auto img_pixels = merge_tiles(img_tiles);

    for (auto &r : img_pixels) {
        for (auto c : r) {
            std::cout << (c ? '#' : '.');
        }
        std::cout << '\n';
    }

    int monsters_cnt = 0;
    for (int transformation = 0; transformation < 8; transformation++) {
        int rotation = transformation % 4;
        bool flip = static_cast<bool>(transformation/4);
        monsters_cnt = count_monsters(img_pixels, rotation, flip);
        if (monsters_cnt) break;
    }
    if (monsters_cnt) {
        std::cout << "Monsters count = " << monsters_cnt << ", occuping " << (15*monsters_cnt) << "px\n";
    }

    int pixels_on_cnt = 0;
    for (size_t i = 0; i < img_pixels.size(); i++) {
        for (size_t j = 0; j < img_pixels[i].size(); j++) {
            if (img_pixels[i][j])
                pixels_on_cnt++;
        }
    }
    std::cout << "Pixels # = " << pixels_on_cnt << "\n";
    std::cout << "Pixels # free of monsters = " << (pixels_on_cnt - 15*monsters_cnt) << std::endl;

    return 0;
}

static std::list<Tile> parse_tiles(std::istream &input) {
    std::list<Tile> tiles_list;

    std::string line;
    while (std::getline(input, line)) {
        Tile tile {std::stoi(line.substr(5, line.length() - 6))};

        for (int i = 0; i < 10; i++) {
            std::getline(input, line);
            if (line.length() != 10) throw std::invalid_argument("Px row is not length = 10");
            for (size_t j = 0; j < line.length(); j++)
                tile.pixels[i][j] = line[j] == '#';
        }
        std::getline(input, line); // blank line

        for (int i = 0; i < 10; i++)
            tile.sides[TileSide::TOP][i] = tile.pixels[0][i];
        for (int i = 0; i < 10; i++)
            tile.sides[TileSide::RIGHT][i] = tile.pixels[i][9];
        for (int i = 0; i < 10; i++)
            tile.sides[TileSide::BOTTOM][i] = tile.pixels[9][9 - i];
        for (int i = 0; i < 10; i++)
            tile.sides[TileSide::LEFT][i] = tile.pixels[9 - i][0];

        tiles_list.push_back(std::move(tile));
    }

    return tiles_list;
}

static std::vector<std::vector<Tile>> arrange_img(int size_side, std::list<Tile> &tiles) {
    std::vector<std::vector<Tile>> img_tiles(size_side, std::vector<Tile>(size_side, -1));

    find_top_left_tile_and_place(img_tiles, tiles);

    for (int i = 0; i < size_side; i++) {
        for (int j = 0; j < size_side; j++) {
            if (i == 0 && j == 0) continue;
            find_tile_and_place(img_tiles, i, j, tiles);
        }
    }

    return img_tiles;
}

static void find_top_left_tile_and_place(std::vector<std::vector<Tile>> &img_tiles, std::list<Tile> &tiles) {
    auto corner_tiles_its = find_corner_tiles(tiles);
    for (auto it : corner_tiles_its) {
        for (int transformation = 0; transformation < 8; transformation++) {
            int rotation = transformation % 4;
            bool flip = static_cast<bool>(transformation/4);
            Tile transformed_tile = get_transformed_tile(*it, rotation, flip);

            auto right_match_it = find_match(transformed_tile, TileSide::RIGHT, tiles).tile_it;
            auto bottom_match_it = find_match(transformed_tile, TileSide::BOTTOM, tiles).tile_it;
            if (right_match_it != tiles.end() && bottom_match_it != tiles.end()) {
                img_tiles[0][0] = transformed_tile;
                tiles.erase(it);
                break;
            }
        }
        if (img_tiles[0][0].num != -1)
            break;
    }
}

static std::list<std::list<Tile>::iterator> find_corner_tiles(std::list<Tile> &tiles) {
    static std::array<TileSide,4> sides = {TileSide::TOP, TileSide::RIGHT, TileSide::BOTTOM, TileSide::LEFT};

    std::list<std::list<Tile>::iterator> tiles_its_to_extract;
    for (auto it = tiles.begin(); it != tiles.end(); it++) {
        unsigned int sides_with_match = 0;
        for (size_t i = 0; i < sides.size(); i++) {
            if (find_match(*it, sides[i], tiles).tile_it != tiles.end())
                sides_with_match++;
        }
        if (sides_with_match == 2) {
            tiles_its_to_extract.push_back(it);
        }
    }
    return tiles_its_to_extract;
}

static void find_tile_and_place(std::vector<std::vector<Tile>> &img_tiles, int i, int j, std::list<Tile> &tiles) {
    Tile &compare_tile = j != 0 ? img_tiles[i][j - 1] : img_tiles[i - 1][j];
    TileSide compare_side = j != 0 ? TileSide::RIGHT : TileSide::BOTTOM;

    TileMatch match = find_match(compare_tile, compare_side, tiles);
    if (match.tile_it == tiles.end())
        throw std::logic_error("Not found");

    Tile matched_tile_transformed = get_transformed_tile(*match.tile_it, match.rotation, match.flip);
    if (i > 0 && !match_tiles_sides(matched_tile_transformed, TileSide::TOP, img_tiles[i - 1][j]))
        throw std::logic_error("Found doesn't match top");
    if (j > 0 && !match_tiles_sides(matched_tile_transformed, TileSide::LEFT, img_tiles[i][j - 1]))
        throw std::logic_error("Found doesn't match left " + std::to_string(i) + "," + std::to_string(j));
    
    img_tiles[i][j] = matched_tile_transformed;
    tiles.erase(match.tile_it);
}

static TileMatch find_match(const Tile &tile, TileSide side, const std::list<Tile> &tiles) {
    for (auto it = tiles.begin(); it != tiles.end(); it++) {
        if (it->num != tile.num) {
            for (int transformation = 0; transformation < 8; transformation++) {
                int rotation = transformation % 4;
                bool flip = static_cast<bool>(transformation/4);
                Tile transformed_tile = get_transformed_tile(*it, rotation, flip);
                if (match_tiles_sides(tile, side, transformed_tile))
                    return TileMatch{it,rotation,flip};
            }
        }
    }
    return TileMatch{tiles.end(), 0, false};
}

static Tile get_transformed_tile (const Tile &orig, int rots_cws_cnt, bool flip) {
    Tile tile{orig.num};

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            int i0, j0;
            switch (rots_cws_cnt) {
                case 0: i0 = i; j0 = j; break;
                case 1: i0 = 9 - j; j0 = i; break;
                case 2: i0 = 9 - i; j0 = 9 - j; break;
                case 3: i0 = j; j0 = 9 - i; break;
            }
            if (flip) i0 = 9 - i0;
            tile.pixels[i][j] = orig.pixels[i0][j0];
        }
    }

    for (int i = 0; i < 10; i++)
        tile.sides[TileSide::TOP][i] = tile.pixels[0][i];
    for (int i = 0; i < 10; i++)
        tile.sides[TileSide::RIGHT][i] = tile.pixels[i][9];
    for (int i = 0; i < 10; i++)
        tile.sides[TileSide::BOTTOM][i] = tile.pixels[9][9 - i];
    for (int i = 0; i < 10; i++)
        tile.sides[TileSide::LEFT][i] = tile.pixels[9 - i][0];

    return tile;
}

static std::vector<std::vector<bool>> merge_tiles (std::vector<std::vector<Tile>> &img_tiles) {
    size_t px_side_size = 8 * img_tiles.size();

    std::vector<std::vector<bool>> pixels(
        px_side_size,
        std::vector<bool>(px_side_size)
    );
    
    for (size_t i = 0; i < px_side_size; i++) {
        for (size_t j = 0; j < px_side_size; j++) 
            pixels[i][j] = img_tiles[i/8][j/8].pixels[i%8+1][j%8+1];
    }

    return pixels;
}

/*
                  # 
#    ##    ##    ###
 #  #  #  #  #  #   
*/
static int count_monsters (const std::vector<std::vector<bool>> &pixels, int rot_cnt_cws, bool flip) {
    constexpr int MONSTER_COLS_LENGTH = 20;
    constexpr int MONSTER_ROWS_LENGTH = 3;
    static std::pair<int,int> monster_px_on [] {
        {0,18},
        {1,0}, {1,5}, {1,6}, {1,11}, {1,12}, {1,17}, {1,18}, {1,19},
        {2,1}, {2,4}, {2,7}, {2,10}, {2,13}, {2,16}
    };

    size_t px_side_size = pixels.size();
    std::vector<std::vector<bool>> pixels_transformed(
        px_side_size,
        std::vector<bool>(px_side_size)
    );
    for (size_t i = 0; i < px_side_size; i++) {
        for (size_t j = 0; j < px_side_size; j++) {
            int i0, j0;
            switch (rot_cnt_cws) {
                case 0: i0 = i; j0 = j; break;
                case 1: i0 = px_side_size - 1 - j; j0 = i; break;
                case 2: i0 = px_side_size - 1 - i; j0 = px_side_size - 1 - j; break;
                case 3: i0 = j; j0 = px_side_size - 1 - i; break;
            }
            if (flip) i0 = px_side_size - 1 - i0;
            pixels_transformed[i][j] = pixels[i0][j0];
        }
    }

    int monsters_cnt = 0;
    for (size_t i = 0; i <= pixels_transformed.size() - MONSTER_ROWS_LENGTH; i++) {
        for (size_t j = 0; j <= pixels_transformed[i].size() - MONSTER_COLS_LENGTH; j++) {
            bool matched = true;
            for (size_t k = 0; k < 15; k++) {
                size_t i_check = i + monster_px_on[k].first;
                size_t j_check = j + monster_px_on[k].second;
                if (!pixels_transformed[i_check][j_check]) {
                    matched = false;
                    break;
                }
            }
            if (matched)
                monsters_cnt++;
        }
    }
    return monsters_cnt;
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

static inline bool is_corner_pos(int i, int j, int side_size) {
    return (i == 0 || i == side_size - 1) && (j == 0 || j == side_size - 1);
}

static inline bool is_border_pos(int i, int j, int side_size) {
    return i == 0 || i == side_size - 1 || j == 0 || j == side_size - 1;
}

static inline unsigned int calc_square_root(unsigned int num) {
    size_t result = 1;
    while (result * result < num)
        result++;
    if (result * result != num)
        throw std::logic_error("Cannot calc square root of " + std::to_string(num));
    return result;
}
