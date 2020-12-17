#include <iostream>
#include <string>
#include <vector>
#include <map>

static constexpr int BOOT_CYCLES_NUM = 6;

struct axis_size { int beg, end; };
using Row = std::map<int,bool>;
using Grid2D = std::map<int,Row>;
using Grid3D = std::map<int,Grid2D>;
using Grid4D = std::map<int,Grid3D>;

static int count_adjacents_on(Grid4D &grid, int w_orig, int z_orig, int y_orig, int x_orig);

int main () {
    std::istream &input = std::cin;
    axis_size w_size = {0, 1}, z_size = {0, 1}, y_size = {0, 1}, x_size = {0, 1};
    std::vector<Grid4D> cycles_grids(BOOT_CYCLES_NUM + 1);

    std::string line;
    for (int y = 0; std::getline(input, line); y++) {
        for (int x = 0; x < static_cast<int>(line.length()); x++) {
            cycles_grids[0][0][0][y][x] = line[x] == '#';
            x_size.end = x + 1;
        }
        y_size.end = y + 1;
    }

    for (int cycle = 1; cycle <= BOOT_CYCLES_NUM; cycle++) {
        w_size = {w_size.beg - 1, w_size.end + 1};
        z_size = {z_size.beg - 1, z_size.end + 1};
        y_size = {y_size.beg - 1, y_size.end + 1};
        x_size = {x_size.beg - 1, x_size.end + 1};
        Grid4D &grid_now = cycles_grids[cycle];
        Grid4D &grid_prev = cycles_grids[cycle - 1];
        for (int w = w_size.beg; w < w_size.end; w++) {
            for (int z = z_size.beg; z < z_size.end; z++) {
                for (int y = y_size.beg; y < y_size.end; y++) {
                    for (int x = x_size.beg; x < x_size.end; x++) {
                        bool cube_on = grid_prev[w][z][y][x];
                        int adjacents_on_cnt = count_adjacents_on(grid_prev, w, z, y, x);
                        if (cube_on) {
                            if (adjacents_on_cnt == 2 || adjacents_on_cnt == 3)
                                grid_now[w][z][y][x] = true;
                            else
                                grid_now[w][z][y][x] = false;
                        }
                        else {
                            if (adjacents_on_cnt == 3)
                                grid_now[w][z][y][x] = true;
                            else
                                grid_now[w][z][y][x] = false;
                        }
                    }
                }
            }
        }
    }

    int cnt = 0;
    Grid4D &grid = cycles_grids.back();
    for (int w = w_size.beg; w < w_size.end; w++) {
        for (int z = z_size.beg; z < z_size.end; z++) {
            for (int y = y_size.beg; y < y_size.end; y++) {
                for (int x = x_size.beg; x < x_size.end; x++) {
                    if (grid[w][z][y][x])
                        cnt++;
                }
            }
        }
    }

    std::cout << "Cubes ON count = " << cnt << std::endl;
    return 0;
}


static int count_adjacents_on(Grid4D &grid, int w_orig, int z_orig, int y_orig, int x_orig) {
    int cnt = 0;
    for (int w = w_orig - 1; w <= w_orig + 1; w++) {
        for (int z = z_orig - 1; z <= z_orig + 1; z++) {
            for (int y = y_orig - 1; y <= y_orig + 1; y++) {
                for (int x = x_orig - 1; x <= x_orig + 1; x++) {
                    if ((w != w_orig || z != z_orig || y != y_orig || x != x_orig) && grid[w][z][y][x])
                        cnt++;
                }
            }
        }
    }
    return cnt;
}