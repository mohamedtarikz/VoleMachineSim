#include "VoleMachineSim.h"
#include <iostream>

// Constructor to initialize the Machine with default memory and CPU
Machine::Machine() : mem(), cpu() {}

// Constructor to initialize the Machine with instructions from a file
Machine::Machine(fstream& instructions) : mem(instructions), cpu() {}

// Constructor to initialize the Machine with instructions from a vector
Machine::Machine(vector<string> instructions) : mem(std::move(instructions)), cpu() {}

// Method to get a reference to the CPU
CPU& Machine::getCPU() {
    return cpu;
}

// Method to get a reference to the Register
Register& Machine::getRegister() {
    return cpu.getRegister();
}

// Method to get a reference to the Memory
Memory& Machine::getMemory() {
    return mem;
}