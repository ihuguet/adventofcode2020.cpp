#include <iostream>
#include <string>
#include <regex>
#include <stdexcept>

int main ()
{
  std::string str, pwd;
  std::regex pwd_regex(R"(^(\d+)-(\d+) ([a-zA-Z]): (.*)$)");
  std::smatch pwd_match;
  size_t cnt_pwd_total = 0, cnt_pwd_valid = 0;
  
  while (std::getline(std::cin, str)) {
      if (std::regex_match(str, pwd_match, pwd_regex)) {
          int idx1 = std::stoi(pwd_match[1]) - 1;
          int idx2 = std::stoi(pwd_match[2]) - 1;
          char character = *pwd_match[3].first;
          pwd = pwd_match[4].str();
          
          //std::cout << str << "\n";
          //std::cout << "min-max: " << min << "-" << max << ", ch: " << character << ", pwd: " << pwd_match[4] << "\n\n";
          
          size_t cnt = 0;
          try {
              if (character == pwd.at(idx1)) cnt++;
              if (character == pwd.at(idx2)) cnt++;
          }
          catch (std::out_of_range &e) {
              std::cerr << "Out of range idx : " << str << std::endl;
          }
          
          if (cnt == 1) {
              cnt_pwd_valid++;
          }
          cnt_pwd_total++;
      }
      else {
          std::cerr << "Invalid line: " << str << "\n";
      }
  }
  
  std::cout << "Valid passwords: " << cnt_pwd_valid << "/" << cnt_pwd_total << std::endl;
  return 0;
}
