#include "VoleMachineSim.h"

CPU::CPU() {
    PC = 0;
}

void CPU::fetch(Memory& mem) {
    IR = mem.getCell(PC) + mem.getCell(PC+1);
    PC+=2;
}

vector<int> CPU::decode() {
    vector<int> decoded;
    decoded.emplace_back(ALU::hexToDec(IR.substr(0, 1)));
    if(decoded[0] == 4){
        decoded.emplace_back(ALU::hexToDec(IR.substr(2,1)));
        decoded.emplace_back(ALU::hexToDec(IR.substr(3,1)));
    }
    else if(decoded[0] == 5 || decoded[0] == 6){
        decoded.emplace_back(ALU::hexToDec(IR.substr(1,1)));
        decoded.emplace_back(ALU::hexToDec(IR.substr(2,1)));
        decoded.emplace_back(ALU::hexToDec(IR.substr(3,1)));
    }
    else{
        decoded.emplace_back(ALU::hexToDec(IR.substr(1,1)));
        decoded.emplace_back(ALU::hexToDec(IR.substr(2,2)));
    }
    return decoded;
}

void CPU::execute(Register& reg, Memory& mem, vector<int> instruction) {
    if(instruction[0] == 1){
        cu.load(instruction[1], instruction[2], reg, mem);
    }
    else if(instruction[0] == 2){
        cu.load(instruction[1], instruction[2], reg);
    }
    else if(instruction[0] == 3){
        cu.store(instruction[1], instruction[2], reg, mem);
    }
    else if(instruction[0] == 4){
        cu.move(instruction[1], instruction[2], reg);
    }
    else if(instruction[0] == 5){
        alu.add(instruction[1], instruction[2], instruction[3], reg);
    }
    else if(instruction[0] == 6){

    }
    else if(instruction[0] == 11){
        cu.jump(instruction[1], instruction[2], reg, PC);
    }
    else if(instruction[0] == 12){
        cu.halt();
    }
}

void CPU::runNextStep(Memory& mem) {
    fetch(mem);
    if(alu.isValid(IR)) {
        vector<int> instruction = decode();
        execute(reg, mem, instruction);
    }
}

Register CPU::getRegister() {
    return reg;
}
