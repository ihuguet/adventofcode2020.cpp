#include <iostream>
#include <string>
#include <vector>

struct grid_elem {
    bool chair : 1;
    bool occupy : 1;
    uint8_t visible_chairs_occupied : 6;
};

std::vector<std::vector<grid_elem>> grid;

/// inc visible_chairs_occupied in first chair found from [row_orig, col_orig] in direction [row_inc, col_inc]
static void inc_visible_chair_occupancy (ssize_t row_orig, ssize_t col_orig, ssize_t row_inc, ssize_t col_inc);


int main () {
    std::istream &input = std::cin;

    std::string line;
    while (std::getline(input, line)) {
        auto &row = grid.emplace_back();
        for (char ch : line)
            row.emplace_back(grid_elem{ch == 'L', false, 0});
    }
    ssize_t i_max = grid.size() - 1;
    ssize_t j_max = grid[0].size() - 1;

    while (true) {
        bool any_seat_chgd = false;

        // occupy / deoccupy chairs
        for (ssize_t i = 0; i <= i_max; i++) {
            for (ssize_t j = 0; j <= j_max; j++) {
                grid_elem &elem = grid[i][j];
                if (!elem.chair)
                    continue;
                else if (elem.occupy && elem.visible_chairs_occupied >= 5)
                    elem.occupy = false, any_seat_chgd = true;
                else if (!elem.occupy && elem.visible_chairs_occupied == 0)
                    elem.occupy = true, any_seat_chgd = true;
            }
        }

        // end condition: no chairs were occupied / deoccupied
        if (!any_seat_chgd)
            break;

        // reset occupancy counts
        for (ssize_t i = 0; i <= i_max; i++) {
            for (ssize_t j = 0; j <= j_max; j++) {
                grid[i][j].visible_chairs_occupied = 0;
            }
        }

        // calculate new occupancy counts
        for (ssize_t i = 0; i <= i_max; i++) {
            for (ssize_t j = 0; j <= j_max; j++) {
                if (!grid[i][j].occupy)
                    continue;
                inc_visible_chair_occupancy(i, j, -1, -1);
                inc_visible_chair_occupancy(i, j, -1,  0);
                inc_visible_chair_occupancy(i, j, -1,  1);
                inc_visible_chair_occupancy(i, j,  0, -1);
                inc_visible_chair_occupancy(i, j,  0,  1);
                inc_visible_chair_occupancy(i, j,  1, -1);
                inc_visible_chair_occupancy(i, j,  1,  0);
                inc_visible_chair_occupancy(i, j,  1,  1);
            }
        }
    }

    size_t occupied_cnt = 0;
    for (auto &row : grid) {
        for (auto &elem : row) {
            if (elem.occupy)
                occupied_cnt++;
        }
    }

    std::cout << "Occupied chairs = " << occupied_cnt << std::endl;
    return 0;
}

static void inc_visible_chair_occupancy (ssize_t row_orig, ssize_t col_orig, ssize_t row_inc, ssize_t col_inc) {
    ssize_t row = row_orig + row_inc;
    ssize_t col = col_orig + col_inc;
    ssize_t row_lim = row_inc == 1 ? grid.size()    : -1;
    ssize_t col_lim = col_inc == 1 ? grid[0].size() : -1;

    while (row != row_lim && col != col_lim) {
        if (grid[row][col].chair) {
            grid[row][col].visible_chairs_occupied++;
            break;
        }
        row += row_inc;
        col += col_inc;
    }
}
