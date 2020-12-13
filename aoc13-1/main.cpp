#include <iostream>
#include <string>
#include <limits>

int main () {
    std::istream &input = std::cin;
    int earliest_departure_time = std::numeric_limits<int>::max();
    int earliest_departure_bus;
    std::string str;

    std::getline(input, str);
    int current_time = std::stoi(str);

    while (std::getline(input, str, ',')) {
        if (str != "x") {
            int bus_line = std::stoi(str);
            int sum_val = current_time % bus_line == 0 ? 0 : 1;
            int bus_departure_time = bus_line * (current_time / bus_line + sum_val);
            if (bus_departure_time < earliest_departure_time) {
                earliest_departure_time = bus_departure_time;
                earliest_departure_bus = bus_line;
            }
        }
    }

    std::cout << "Earliest departure: bus line " << earliest_departure_bus << " on timestamp " << earliest_departure_time << '\n';
    std::cout << "Wait time = " << (earliest_departure_time - current_time) << '\n';
    std::cout << "Bus line * wait time = " << ((earliest_departure_time - current_time) * earliest_departure_bus) << std::endl;
    return 0;
}
