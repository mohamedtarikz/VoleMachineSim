#include "VoleMachineSim.h"

Register::Register() {
    for (int i = 0; i < size; i++) {
        memory[i] = 0;
    }
}

int Register::getCell(int index) {
    assert(index >= 0 && index < size);
    return memory[index];
}

void Register::setCell(int index, int value) {
    assert(index >= 0 && index < size);
    memory[index] = value;
}