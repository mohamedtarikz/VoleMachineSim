#include "VoleMachineSim.h"
#include <iostream>
#include <thread>

// Load a value from memory into a register
void CU::load(int idxRegister, int idxMemory, Register &reg, Memory &mem) {
    reg.setCell(idxRegister, ALU::hexToDec(mem.getCell(idxMemory)));
}

// Load a direct value into a register
void CU::load(int idxRegister, int value, Register &reg) {
    reg.setCell(idxRegister, value);
}

// Store a value from a register into memory
void CU::store(int idxRegister, int idxMemory, Register &reg, Memory &mem) {
    mem.setCell(idxMemory, ALU::decToHex(reg.getCell(idxRegister)));
    // Print the value if stored in memory cell 0
    if(idxMemory == 0){
        std::cout << mem.getCell(idxMemory) << std::endl;
    }
}

// Move a value from one register to another
void CU::move(int idxRegister1, int idxRegister2, Register &reg) {
    reg.setCell(idxRegister2, reg.getCell(idxRegister1));
}

// Jump to a memory address if the value in the register matches the value in register 0
void CU::jump(int idxRegister, int idxMemory, Register &reg, int& PC) {
    if(reg.getCell(idxRegister) != reg.getCell(0)){
        return;
    }
    PC = idxMemory;
}

// Halt the execution and output the state of registers and memory
void CU::halt(Register& reg, Memory& mem) {
    MainUI::outputState(reg, mem);
    std::this_thread::sleep_for(std::chrono::seconds(8));
    exit(0);
}