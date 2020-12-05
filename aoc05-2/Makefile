PUZZLE=$(notdir $(realpath .))
$(info $(PUZZLE))

all:
	mkdir -p ../build
	g++ -std=c++17 -Wall -pedantic -Wextra main.cpp -o ../build/$(PUZZLE)
