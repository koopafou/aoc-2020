#include <regex>
#include "../../utils.hh"
#define DAY_08
#define VM_DEBUG

enum Error {
	ERROR_PC_OOB,
	ERROR_UNKNOWN_INSTR,

	ERROR_N,
};

enum Opcode {
	OP_ACC,
	OP_NOP,
	OP_JMP,

	OP_N
};

struct Instr {
	Opcode op;
	i64 arg;
};

struct VM {
	std::vector<Instr> program;
	i64 accumulator;
	i64 pc;

	void init (const std::vector<Instr>& prog) {
		program = prog;
		accumulator = 0;
		pc = 0;
	}

	Error run () {
		while (true) {
			if (pc < 0 || program.size() <= pc) {
#ifdef VM_DEBUG
				std::cerr << "PC OOB" << std::endl;
#endif
				return ERROR_PC_OOB;
			}
#ifdef DAY_08
			i64 pc_save = pc;
#endif
			switch (program[pc].op) {
			case OP_ACC:
#ifdef VM_DEBUG
				std::cerr << "acc : " << accumulator << " + " << program[pc].arg << " = ";
#endif
				accumulator += program[pc].arg;
#ifdef VM_DEBUG
				std::cerr << accumulator << std::endl;
#endif
				++pc;
				break;
			case OP_NOP:
#ifdef VM_DEBUG
				std::cerr << "nop" << std::endl;
#endif
				++pc;
				break;
			case OP_JMP:
#ifdef VM_DEBUG
				std::cerr << "jmp : " << pc << " + " << program[pc].arg << " = ";
#endif
				pc += program[pc].arg;
#ifdef VM_DEBUG
				std::cerr << pc << std::endl;
#endif
				break;
			case OP_N:
#ifdef VM_DEBUG
				std::cerr << "unknown" << std::endl;
#endif
				return ERROR_UNKNOWN_INSTR;
				break;
			}
#ifdef DAY_08
			program[pc_save].op = OP_N;
#endif
		}
		return ERROR_N;
	}
};

int main () {
	std::map<std::string, Opcode> ops;
	ops["acc"] = OP_ACC;
	ops["nop"] = OP_NOP;
	ops["jmp"] = OP_JMP;

	std::vector<Instr> program;
	std::regex reg("([a-z]+) ([+-][0-9]+)");
	std::smatch sma;
	program.reserve(1000);
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		if (line.empty())
			break;
		if (!std::regex_match(line, sma, reg))
			return 1;
		auto it = ops.find(sma[1].str());
		if (it == ops.end())
			return 1;
		program.push_back(Instr{it->second, std::stoi(sma[2].str())});
#ifdef VM_DEBUG
		std::cerr << "<<" << line << ">>" << std::endl;
#endif
	}

	VM vm;

	vm.init(program);
	vm.run();
	std::cout << vm.accumulator << std::endl;

	for (u32 i = 0; i < program.size(); ++i) {
		if (program[i].op != OP_NOP && program[i].op != OP_JMP)
			continue;
		vm.init(program);
		vm.program[i].op = (program[i].op == OP_NOP ? OP_JMP : OP_NOP);
		if (vm.run() == ERROR_PC_OOB) {
			std::cout << vm.accumulator << std::endl;
			//break;
		}
	}

	return 0;
}
