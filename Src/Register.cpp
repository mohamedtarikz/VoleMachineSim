#include "VoleMachineSim.h"

Register::Register() {
    for (int i = 0; i < size; i++) {
        memory[i] = 0;
    }
}

int Register::getCell(int index) {
    if(!(index >= 0 && index < size)){
        throw runtime_error("Index out of range");
    }
    return memory[index];
}

void Register::setCell(int index, int value) {
    if(!(index >= 0 && index < size)){
        throw runtime_error("Index out of range");
    }
    memory[index] = value;
}