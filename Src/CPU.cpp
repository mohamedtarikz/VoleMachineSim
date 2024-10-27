#include "VoleMachineSim.h"
#include <iostream>

// Constructor to initialize the CPU
CPU::CPU() : reg() {
    PC = 0; // Initialize Program Counter to 0
}

// Method to get the current instruction register value
string CPU::getIR() {
    return IR;
}

// Method to get the current program counter value
int CPU::getPC() {
    return PC;
}

// Method to fetch the next instruction from memory
void CPU::fetch(Memory& mem) {
    if(PC >= 255) { // Check if the program counter has reached the end
        cout<<"Program Counter has reached the end."<<endl;
        cu.halt(reg, mem, PC, IR); // Halt the execution
    }
    // Fetch two consecutive memory cells and concatenate them to form the instruction
    IR = mem.getCell(PC) + mem.getCell(PC+1);
    PC += 2; // Increment the program counter by 2
}

// Method to decode the fetched instruction
vector<int> CPU::decode() {
    vector<int> decoded;
    // Decode the first part of the instruction
    decoded.emplace_back(ALU::hexToDec(IR.substr(0, 1)));

    // Decode based on the instruction type
    if(decoded[0] == 4) {
        decoded.emplace_back(ALU::hexToDec(IR.substr(2, 1)));
        decoded.emplace_back(ALU::hexToDec(IR.substr(3, 1)));
    } else if(decoded[0] == 5 || decoded[0] == 6) {
        decoded.emplace_back(ALU::hexToDec(IR.substr(1, 1)));
        decoded.emplace_back(ALU::hexToDec(IR.substr(2, 1)));
        decoded.emplace_back(ALU::hexToDec(IR.substr(3, 1)));
    } else {
        decoded.emplace_back(ALU::hexToDec(IR.substr(1, 1)));
        decoded.emplace_back(ALU::hexToDec(IR.substr(2, 2)));
    }
    return decoded;
}

// Method to execute the decoded instruction
void CPU::execute(Register& reg, Memory& mem, vector<int> instruction) {
    // Execute based on the instruction type
    if(instruction[0] == 1) {
        cu.load(instruction[1], instruction[2], reg, mem);
    } else if(instruction[0] == 2) {
        cu.load(instruction[1], instruction[2], reg);
    } else if(instruction[0] == 3) {
        cu.store(instruction[1], instruction[2], reg, mem);
    } else if(instruction[0] == 4) {
        cu.move(instruction[1], instruction[2], reg);
    } else if(instruction[0] == 5) {
        alu.sumTwosComplement(instruction[1], instruction[2], instruction[3], reg);
    } else if(instruction[0] == 6) {
        alu.sumFloatingPoint(instruction[1], instruction[2], instruction[3], reg);
    } else if(instruction[0] == 11) {
        cu.jump(instruction[1], instruction[2], reg, PC);
    } else if(instruction[0] == 12) {
        cu.halt(reg, mem, PC, IR);
    }
}

// Method to run the next step of the program
void CPU::runNextStep(Memory& mem) {
    if(PC >= 255) {// Check if the program counter has reached the end
        cout<<"Program Counter has reached the end."<<endl;
        cu.halt(reg, mem, PC, IR); // Halt the execution
    }
    fetch(mem); // Fetch the next instruction
    if(alu.isValid(IR)) { // Check if the instruction is valid
        vector<int> instruction = decode(); // Decode the instruction
        execute(reg, mem, instruction); // Execute the instruction
    }
}

// Method to get the register object
Register& CPU::getRegister() {
    return reg;
}