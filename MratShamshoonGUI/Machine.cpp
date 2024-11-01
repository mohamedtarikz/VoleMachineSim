#include "VoleMachine.h"

// Constructor to initialize the Machine with default memory and CPU
Machine::Machine() : mem(), cpu() {}

// Method to get a reference to the CPU
CPU& Machine::getCPU() {
    return cpu;

    // emit cpu.CPUupdated();
}

// Method to get a reference to the Register
Register& Machine::getRegister() {
    return cpu.getRegister();
}

// Method to get a reference to the Memory
Memory& Machine::getMemory() {
    return mem;
}

void Machine::clear(){
    mem.clear();
    cpu.clear();
}

void Machine::reset(){
    cpu.clear();
}

void Machine::play(){
    while(cpu.getPC() <= 254){
        cpu.runNextStep(mem);
    }
}
