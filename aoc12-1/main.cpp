#include <iostream>
#include <string>

struct coords {
    int lat, lon;
};

const coords operator+ (coords a, coords b) {
    return coords{a.lat + b.lat, a.lon + b.lon};
}

enum direction {
    EAST = 0, NORTH = 90, WEST = 180, SOUTH = 270
};

class Boat {
public:
    explicit Boat() : pos{0, 0}, rot(direction::EAST) {}
    coords get_pos() { return pos; }
    direction get_rotation() { return rot; }
    coords move(coords coords_chg);
    coords move(int dist, direction dir);
    direction rotate(int degrees, bool clockwise = true);
private:
    coords pos;
    direction rot;
};


int main () {
    std::istream &input = std::cin;
    Boat boat;

    std::string line;
    while (std::getline(input, line)) {
        int quantity = std::stoi(line.substr(1));
        switch (line[0]) {
            case 'N': boat.move(quantity, direction::NORTH); break;
            case 'S': boat.move(quantity, direction::SOUTH); break;
            case 'E': boat.move(quantity, direction::EAST); break;
            case 'W': boat.move(quantity, direction::WEST); break;
            case 'F': boat.move(quantity, boat.get_rotation()); break;
            case 'L': boat.rotate(quantity, true); break;
            case 'R': boat.rotate(quantity, false); break;
            default:
                std::cerr << "Invalid line: " << line;
                break;
        }
    }

    auto [lat, lon] = boat.get_pos();
    std::cout << "Final pos = " << lat << ", " << lon << '\n';
    std::cout << "Manhattan dist = " << (std::abs(lat) + std::abs(lon)) << std::endl;
    return 0;
}

coords Boat::move (coords coords_chg) {
    pos = pos + coords_chg;
    return pos;
}

coords Boat::move(int dist, direction dir) {
    switch (dir) {
        case direction::EAST:  return move(coords{0, -dist});
        case direction::NORTH: return move(coords{dist, 0});
        case direction::WEST:  return move(coords{0, dist});
        case direction::SOUTH: return move(coords{-dist, 0});
    }
    return {0, 0};
}

direction Boat::rotate (int degrees, bool clockwise) {
    if (degrees % 90 != 0)
        throw std::invalid_argument("Rotation must be multiple of 90º");
    if (!clockwise)
        degrees = -degrees;

    rot = static_cast<direction>(static_cast<int>(rot) + degrees);
    if (rot > 270)
        rot = static_cast<direction>(static_cast<int>(rot) - 360);
    else if (rot < 0)
        rot = static_cast<direction>(static_cast<int>(rot) + 360);

    return rot;
}