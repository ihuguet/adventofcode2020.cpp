#include <iostream>
#include <string>
#include <vector>

enum class operation {ACC, JMP, NOP};
struct instruction {
    explicit instruction(operation op, int arg) : op(op), argument(arg) {}
    operation op;
    int argument;
};

static std::vector<instruction> parseAsm(std::istream &input);
static long executeProgram (const std::vector<instruction> &instructions);
static operation strToOp(const std::string &opStr);

int main () {
    std::istream &input = std::cin;

    std::vector<instruction> instructions = parseAsm(input);

    size_t instruction_to_repair = 0;
    long accumulator;
    while (instruction_to_repair < instructions.size()) {
        operation &op = instructions[instruction_to_repair].op;
        if (op == operation::JMP || op == operation::NOP) {
            op = op == operation::NOP ? operation::JMP : operation::NOP;
            try {
                accumulator = executeProgram(instructions);
                break;
            }
            catch (std::logic_error &) {}
            op = op == operation::NOP ? operation::JMP : operation::NOP;
        }
        instruction_to_repair++;
    }

    if (instruction_to_repair >= instructions.size()) {
        std::cerr << "Instruction to repair not found" << std::endl;
        return 1;
    }

    std::cout << "Instruction repaired in line " << (instruction_to_repair + 1) << "\n";
    std::cout << "Accumulator after boot sequence: " << accumulator << std::endl;
    return 0;
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

static long executeProgram (const std::vector<instruction> &instructions) {
    std::vector<bool> executed_instructions(instructions.size());

    long accumulator = 0;
    size_t pc = 0;
    while (pc != instructions.size()) {
        if (pc > instructions.size() || executed_instructions[pc])
            throw std::logic_error("Infinite loop detected in boot sequence");

        executed_instructions[pc] = true;

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

    return accumulator;
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

