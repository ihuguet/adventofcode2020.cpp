#include <iostream>
#include <string>
#include <vector>

enum class operation {ACC, JMP, NOP};

struct instruction {
    explicit instruction(operation op, int arg)
        : op(op), argument(arg), times_executed(0) {}
    operation op;
    int argument;
    unsigned int times_executed;
};

static operation strToOp(const std::string &opStr);
static std::vector<instruction> parseAsm(std::istream &input);

int main () {
    std::istream &input = std::cin;

    std::vector<instruction> instructions = parseAsm(input);

    long accumulator = 0;
    size_t pc = 0;
    while (true) {
        if (instructions[pc].times_executed > 0)
            break;
        instructions[pc].times_executed++;
        switch (instructions[pc].op) {
            case operation::ACC:
                accumulator += instructions[pc].argument;
                pc++;
                break;
            case operation::JMP:
                pc += instructions[pc].argument;
                break;
            case operation::NOP:
                pc++;
                break;
        }
    }

    std::cout << "To be executed 2nd time: line " << pc << "\n";
    std::cout << "Accumulator = " << accumulator << std::endl;
}

static std::vector<instruction> parseAsm(std::istream &input) {
    std::vector<instruction> instructions;

    std::string line;
    while (std::getline(input, line)) {
        operation op = strToOp(line.substr(0, 3));
        int arg = std::stoi(line.substr(4));
        instructions.emplace_back(op, arg);
    }

    return instructions;
}

static operation strToOp(const std::string &opStr) {
    if (opStr == "acc")
        return operation::ACC;
    else if (opStr == "jmp")
        return operation::JMP;
    else if (opStr == "nop")
        return operation::NOP;
    throw std::invalid_argument(std::string("Unknown operation ") + opStr);
}

