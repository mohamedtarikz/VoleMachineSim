#ifndef VOLEMACHINESIM_VOLEMACHINESIM_H
#define VOLEMACHINESIM_VOLEMACHINESIM_H

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
    void loadMemory(fstream& file);
    void loadMemory(vector<string> instructions);
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
    string cnvrtToFloatingPoint(double);
    int cnvrtTwosComplement(string);
public:
    // Method to check if a string is a valid hexadecimal number
    bool isValid(string);
    // Method to convert a hexadecimal string to a decimal integer
    static int hexToDec(string);
    // Method to convert a decimal integer to a hexadecimal string
    static string decToHex(int);
    static int binToDec(string);
    static string decToBin(int, int = 9);
    void sumFloatingPoint(int idxRegister1, int idxRegister2, int idxRegister3, Register& reg);
    void sumTwosComplement(int idxRegister1, int idxRegister2, int idxRegister3, Register& reg);
    // Method to add to register values
    void add(int idxRegister1, int idxRegister2, int idxRegister3, Register& reg);
};

// CU class to handle control operations
class CU{
public:
    // Method to load a value from memory to a register
    void load(int idxRegister, int idxMemory, Register& reg, Memory& mem);
    // Method to load a value to a register
    void load(int idxRegister, int value, Register& reg);
    // Method to store a value from a register to memory
    void store(int idxRegister, int idxMemory, Register& reg, Memory& mem);
    // Method to move a value from one register to another
    void move(int idxRegister1, int idxRegister2, Register& reg);
    // Method to jump to a memory location
    void jump(int idxRegister, int idxMemory, Register& reg, int& PC);
    // Method to halt the program
    void halt();
};

// CPU class to handle the execution of instructions
class CPU {
    int PC; // Program Counter to keep track of the current instruction
    string IR; // Instruction Register to store the current instruction
    Register reg; // Register object to handle register operations
    ALU alu; // ALU object to handle arithmetic and logic operations
    CU cu; // CU object to handle control operations`
    // Method to fetch the next instruction from memory
    void fetch(Memory&);
    // Method to decode the fetched instruction
    vector<int> decode();
    // Method to execute the decoded instruction
    void execute(Register&, Memory&, vector<int>);
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

class MainUI{
    int choice;
    fstream dataFile;
public:
    void DisplayOuputMenu();
    void DisplayoperationMenu();
    int getchoice(int choicese_size);
    void loadinstruction(vector<string>& instructions);
    void PrintRegister(Register& mainRegister);
};

#endif //VOLEMACHINESIM_VOLEMACHINESIM_H