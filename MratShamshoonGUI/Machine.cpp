#include "VoleMachine.h"

// Constructor to initialize the Machine with default memory and CPU
Machine::Machine() : mem(), cpu() {}

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
