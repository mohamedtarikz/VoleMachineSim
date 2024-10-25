#include "VoleMachineSim.h"

Machine::Machine(fstream& file) : mem(file), cpu(){}

Machine::Machine(vector<string>& instructions) : mem(instructions), cpu(){}

pair<Memory, Register> Machine::getState() {
    return make_pair(mem, cpu.getRegister());
}