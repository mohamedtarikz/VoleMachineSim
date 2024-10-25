#include "VoleMachineSim.h"
#include <iostream>

Machine::Machine(fstream& instructions) : mem(instructions), cpu() {}

Machine::Machine(vector<string> instructions) : mem(std::move(instructions)), cpu(){}

void Machine::outputState(Register& reg) {
    for (int i = 0; i < 16; ++i) {
        cout<<"Address 0x"<<ALU::decToHex(i)<<": "<<ALU::decToHex(reg.getCell(i))<<endl;
    }
}

void Machine::outputState(Memory& memory) {
    for (int i = 0; i < 256; i+=2) {
        cout<<"Address 0x"<<ALU::decToHex(i)<<": "<<memory.getCell(i)<<memory.getCell(i+1)<<endl;
    }
}

void Machine::outputState(Register& reg, Memory& memory) {
    cout<<"Register state:"<<endl;
    outputState(reg);
    cout<<"Memory state:"<<endl;
    outputState(memory);
}