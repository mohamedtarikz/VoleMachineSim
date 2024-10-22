#ifndef VOLEMACHINESIM_VOLEMACHINESIM_H
#define VOLEMACHINESIM_VOLEMACHINESIM_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
using namespace std;

// Memory class
class Memory {
    string memory[256]; // Array to store memory cells
    int size = 256; // Size of the memory
public:
    // Constructor to initialize memory from a file
    explicit Memory(fstream&);
    // Constructor to initialize memory from a vector of instructions
    explicit Memory(vector<string>);
    // Method to set a value in a memory cell
    void setCell(int index, string value);
    // Method to get a value from a memory cell
    string getCell(int index);
};

// Register class
class Register {
    int memory[16]; // Array to store register values
    int size = 16; // Size of the register
public:
    // Constructor to initialize the register
    Register();
    // Method to set a value in a register cell
    void setCell(int index, int value);
    // Method to get a value from a register cell
    int getCell(int index);
};

// ALU class to handle arithmetic and logic operations
class ALU {
    // Method to check if a string is a valid hexadecimal number
    bool isValid(string);
    // Method to convert a hexadecimal string to a decimal integer
    int hexToDec(string);
    // Method to convert a decimal integer to a hexadecimal string
    string decToHex(int);
};

// CPU class to handle the execution of instructions
class CPU {
    int PC = 0; // Program Counter to keep track of the current instruction
    string IR; // Instruction Register to store the current instruction
    Register reg; // Register object to handle register operations
    ALU alu; // ALU object to handle arithmetic and logic operations
    // Method to fetch the next instruction from memory
    void fetch(Memory&);
    // Method to decode the fetched instruction
    vector<int> decode();
    // Method to execute the decoded instruction
    void execute(Memory&, vector<int>);
public:
    // Constructor to initialize the CPU
    CPU();
    // Method to run the next step of the program
    void runNextStep(Memory&);
};

// VoleMachineSim class
class VoleMachineSim {
    Memory mem;
    CPU cpu;
public:
    VoleMachineSim();
};

#endif //VOLEMACHINESIM_VOLEMACHINESIM_H