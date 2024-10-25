#include "VoleMachineSim.h"
#include <iostream>
#include <thread>

void CU::load(int idxRegister, int idxMemory, Register &reg, Memory &mem) {
    reg.setCell(idxRegister, ALU::hexToDec(mem.getCell(idxMemory)));
}

void CU::load(int idxRegister, int value, Register &reg) {
    reg.setCell(idxRegister, value);
}

void CU::store(int idxRegister, int idxMemory, Register &reg, Memory &mem) {
    mem.setCell(idxMemory, ALU::decToHex(reg.getCell(idxRegister)));
    if(idxMemory == 0){
        cout<<mem.getCell(idxMemory)<<endl;
    }
}

void CU::move(int idxRegister1, int idxRegister2, Register &reg) {
    reg.setCell(idxRegister2, reg.getCell(idxRegister1));
}

void CU::jump(int idxRegister, int idxMemory, Register &reg, int& PC) {
    if(reg.getCell(idxRegister) != reg.getCell(0)){
        return;
    }
    PC = idxMemory;
}

void CU::halt(Register& reg) {
    MainUI::outputState(reg);
    this_thread::sleep_for(chrono::seconds(8));
    exit(0);
}