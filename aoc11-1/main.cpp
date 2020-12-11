#include <iostream>
#include <string>
#include <vector>

struct grid_elem {
    bool chair : 1;
    bool occupy : 1;
    uint8_t adjacent_occupies : 6;
};

std::vector<std::vector<grid_elem>> grid;

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
                else if (elem.occupy && elem.adjacent_occupies >= 4)
                    elem.occupy = false, any_seat_chgd = true;
                else if (!elem.occupy && elem.adjacent_occupies == 0)
                    elem.occupy = true, any_seat_chgd = true;
            }
        }

        // end condition: no chairs were occupied / deoccupied
        if (!any_seat_chgd)
            break;

        // reset occupancy counts
        for (ssize_t i = 0; i <= i_max; i++) {
            for (ssize_t j = 0; j <= j_max; j++) {
                grid[i][j].adjacent_occupies = 0;
            }
        }

        // calculate new occupancy counts
        for (ssize_t i = 0; i <= i_max; i++) {
            for (ssize_t j = 0; j <= j_max; j++) {
                if (!grid[i][j].occupy)
                    continue;
                ssize_t i_chg_1st  = std::max(i - 1, 0L);
                ssize_t i_chg_last = std::min(i + 1, i_max);
                ssize_t j_chg_1st  = std::max(j - 1, 0L);
                ssize_t j_chg_last = std::min(j + 1, j_max);
                for (ssize_t i_chg = i_chg_1st; i_chg <= i_chg_last; i_chg++) {
                    for (ssize_t j_chg = j_chg_1st; j_chg <= j_chg_last; j_chg++) {
                        if ((i != i_chg || j != j_chg) && grid[i_chg][j_chg].chair)
                            grid[i_chg][j_chg].adjacent_occupies++;
                    }
                }
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
