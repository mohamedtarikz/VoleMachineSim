#include "VoleMachine.h"

// Load a value from memory into a register
void CU::load(int idxRegister, int idxMemory, Register &reg, Memory &mem) {
    reg.setCell(idxRegister, ALU::hexToDec(mem.getCell(idxMemory)));
}

// Load a direct value into a register
void CU::load(int idxRegister, int value, Register &reg) {
    reg.setCell(idxRegister, value);
}

// Store a value from a register into memory
string CU::store(int idxRegister, int idxMemory, Register &reg, Memory &mem) {
    mem.setCell(idxMemory, ALU::decToHex(reg.getCell(idxRegister)));
    // Print the value if stored in memory cell 0
    if(idxMemory == 0){
        return mem.getCell(idxMemory);
    }
    return "";
}

// Move a value from one register to another
void CU::move(int idxRegister1, int idxRegister2, Register &reg) {
    reg.setCell(idxRegister2, reg.getCell(idxRegister1));
}

// Jump to a memory address if the value in the register matches the value in register 0
void CU::jump(bool choice, int idxRegister, int idxMemory, Register &reg, int& PC) {
    if(choice){
        if(ALU::cnvrtTwosComplement(ALU::decToBin(reg.getCell(idxRegister))) <= ALU::cnvrtTwosComplement(ALU::decToBin(reg.getCell(0)))){
            return;
        }
        PC = idxMemory;
    }
    else{
        if(reg.getCell(idxRegister) != reg.getCell(0)){
            return;
        }
        PC = idxMemory;
    }
}

// Halt the execution and output the state of registers and memory
string CU::halt(int& PC) {
    PC = 256;
    return "Program has finished...";
}
