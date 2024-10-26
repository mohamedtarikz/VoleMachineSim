#include "VoleMachineSim.h"
#include <iostream>

Machine::Machine() : mem(), cpu() {}

Machine::Machine(fstream& instructions) : mem(instructions), cpu() {}

Machine::Machine(vector<string> instructions) : mem(std::move(instructions)), cpu(){}

CPU& Machine::getCPU() {
    return cpu;
}

Register& Machine::getRegister() {
    return cpu.getRegister();
}

Memory& Machine::getMemory() {
    return mem;
}