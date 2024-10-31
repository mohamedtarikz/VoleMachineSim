#include "VoleMachine.h"
#include <algorithm>
#include <iostream>

// Default constructor to initialize memory with "00"
Memory::Memory(){
    int i = 0;
    while(i < 300){
        memory[i] = "00"; // Initialize each memory cell to "00"
        i++;
    }
}

// Constructor to initialize memory from a file
Memory::Memory(fstream& file){
    string byte;
    int i = 0;
    while(i < 256 && file >> byte){
        if(byte.size() != 4){
            cout << "Invalid instruction: " << byte << endl;
            continue;
        } // Ensure each byte is of size 4
        transform(byte.begin(), byte.end(), byte.begin(), ::toupper); // Convert to uppercase
        string tmp;
        tmp += byte[0];
        tmp += byte[1];
        memory[i] = tmp; // Store the first half of the byte
        tmp = "";
        tmp += byte[2];
        tmp += byte[3];
        memory[i+1] = tmp; // Store the second half of the byte
        i+=2;
    }
}

// Constructor to initialize memory from a vector of instructions
Memory::Memory(vector<string> instructions){
    for(int i = 0; i < (size/2) && i < instructions.size(); i++){
        if(instructions[i].size() != 4){
            cout << "Invalid instruction: " << instructions[i] << endl;
            continue;
        } // Ensure each instruction is of size 4
        string tmp;
        tmp += instructions[i][0];
        tmp += instructions[i][1];
        memory[i*2] = tmp; // Store the first half of the instruction
        tmp = "";
        tmp += instructions[i][2];
        tmp += instructions[i][3];
        memory[i*2+1] = tmp; // Store the second half of the instruction
    }
}

// Method to load memory from a file
void Memory::loadMemory(fstream &file) {
    string byte;
    int i = 0;
    while(i < 256 && file >> byte){
        if(byte.size() != 4){
            cout << "Invalid instruction: " << byte << endl;
            continue;
        } // Ensure each byte is of size 4
        transform(byte.begin(), byte.end(), byte.begin(), ::toupper); // Convert to uppercase
        string tmp;
        tmp += byte[0];
        tmp += byte[1];
        memory[i] = tmp; // Store the first half of the byte
        tmp = "";
        tmp += byte[2];
        tmp += byte[3];
        memory[i+1] = tmp; // Store the second half of the byte
        i+=2;
    }
}

// Method to load memory from a vector of instructions
void Memory::loadMemory(vector<string> instructions) {
    for(int i = 0; i < (size/2) && i < instructions.size(); i++){
        if(instructions[i].size() != 4){
            cout << "Invalid instruction: " << instructions[i] << endl;
            continue;
        } // Ensure each instruction is of size 4
        string tmp;
        tmp += instructions[i][0];
        tmp += instructions[i][1];
        memory[i*2] = tmp; // Store the first half of the instruction
        tmp = "";
        tmp += instructions[i][2];
        tmp += instructions[i][3];
        memory[i*2+1] = tmp; // Store the second half of the instruction
    }
}

// Method to add a single instruction
void Memory::addInstruction(string instruction, int index){
    memory[index] = instruction.substr(0,2);
    memory[index + 1] = instruction.substr(2,2);
}

// Method to set a memory cell value
void Memory::setCell(int index, string value){
    if(value.size() != 2){
        throw runtime_error("Invalid value size");
    } // Ensure the value is of size 2
    if(!(index >= 0 && index < size)){
        throw runtime_error("Invalid memory index");
    } // Ensure the index is within bounds
    memory[index] = value; // Set the memory cell value
}

// Method to get a memory cell value
string Memory::getCell(int index){
    return memory[index]; // Return the memory cell value
}
