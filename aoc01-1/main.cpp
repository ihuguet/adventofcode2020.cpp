#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

const int YEAR = 2020;

int main ()
{
  std::string str;
  std::vector<int> numbers;
  while (std::getline(std::cin, str)) {
    try {
        int current_num = std::stoi(str);
        for (auto num : numbers) {
            if (num + current_num == YEAR) {
                std::cout << num << " + " << current_num << " = " << YEAR << "\n";
                std::cout << num << " · " << current_num << " = " << (num * current_num) << std::endl;
                return 0;
            }
        }
        numbers.push_back(current_num);
    }
    catch (std::invalid_argument &e) {
        std::cerr << "Invalid line (NaN): " << str << std::endl;
    }
  }
  
  std::cerr << "No number pairs found" << std::endl;
  return 1;
}
