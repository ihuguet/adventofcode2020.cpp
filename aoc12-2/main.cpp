#include <iostream>
#include <string>

struct coords {
    long lat, lon;
};

const coords operator+ (coords a, coords b) {
    return coords{a.lat + b.lat, a.lon + b.lon};
}

const coords operator- (coords a, coords b) {
    return coords{a.lat - b.lat, a.lon - b.lon};
}

const coords operator* (coords a, int mult) {
    return coords{mult * a.lat, mult * a.lon};
}

const coords operator* (int mult, coords a) {
    return a * mult;
}

enum direction {
    EAST = 0, NORTH = 90, WEST = 180, SOUTH = 270
};

class Boat {
public:
    explicit Boat() : pos_boat{0, 0}, pos_rel_waypoint{1, -10} {}
    coords get_pos_boat() { return pos_boat; }
    coords get_pos_rel_waypoint() { return pos_rel_waypoint; }
    coords move_waypoint(coords coords_chg);
    coords move_waypoint(int dist, direction dir);
    coords rotate_waypoint(int degrees);
    coords move_towards_waypoint(int dist);
private:
    coords pos_boat, pos_rel_waypoint;
};


int main () {
    std::istream &input = std::cin;
    Boat boat;

    std::string line;
    while (std::getline(input, line)) {
        int quantity = std::stoi(line.substr(1));
        switch (line[0]) {
            case 'N': boat.move_waypoint(quantity, direction::NORTH); break;
            case 'S': boat.move_waypoint(quantity, direction::SOUTH); break;
            case 'E': boat.move_waypoint(quantity, direction::EAST); break;
            case 'W': boat.move_waypoint(quantity, direction::WEST); break;
            case 'F': boat.move_towards_waypoint(quantity); break;
            case 'L': boat.rotate_waypoint(quantity); break;
            case 'R': boat.rotate_waypoint(-quantity); break;
            default:
                std::cerr << "Invalid line: " << line << '\n';
                break;
        }
    }

    auto [lat, lon] = boat.get_pos_boat();
    std::cout << "Final pos = " << lat << ", " << lon << '\n';
    std::cout << "Manhattan dist = " << (std::abs(lat) + std::abs(lon)) << std::endl;
    return 0;
}


coords Boat::move_waypoint(coords coords_chg) {
    pos_rel_waypoint = pos_rel_waypoint + coords_chg;
    return pos_rel_waypoint;
}

coords Boat::move_waypoint(int dist, direction dir) {
    switch (dir) {
        case direction::EAST:  return move_waypoint(coords{0, -dist});
        case direction::NORTH: return move_waypoint(coords{dist, 0});
        case direction::WEST:  return move_waypoint(coords{0, dist});
        case direction::SOUTH: return move_waypoint(coords{-dist, 0});
    }
    return {0, 0};
}

coords Boat::rotate_waypoint (int degrees) {
    if (degrees % 90 != 0)
        throw std::invalid_argument("Rotation must be multiple of 90º");
    while (degrees >= 360)
        degrees -= 360;
    while (degrees < 0)
        degrees += 360;

    coords new_coords;
    switch (degrees) {
        case 90:
            new_coords = {-pos_rel_waypoint.lon, pos_rel_waypoint.lat};
            break;
        case 180:
            new_coords = {-pos_rel_waypoint.lat, -pos_rel_waypoint.lon};
            break;
        case 270:
            new_coords = {pos_rel_waypoint.lon, -pos_rel_waypoint.lat};
            break;
        case 0:
            break;
    }
    pos_rel_waypoint = new_coords;

    return pos_rel_waypoint;
}

coords Boat::move_towards_waypoint(int dist) {
    pos_boat = pos_boat + dist * pos_rel_waypoint;
    return pos_boat;
}
