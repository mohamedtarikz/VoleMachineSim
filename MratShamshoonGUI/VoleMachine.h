#ifndef VOLEMACHINESIM_VOLEMACHINESIM_H
#define VOLEMACHINESIM_VOLEMACHINESIM_H

#include <QWidget>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

// Memory class
class Memory : public QWidget {
    Q_OBJECT
    string memory[300]; // Array to store memory cells
    int size = 256; // Size of the memory
public:
    // Constructor to initialize memory with zeros
    Memory();
    // Method to load memory from a file
    int loadMemory(fstream& file);
    // Method to add a single instruction
    void addInstruction(string instruction, int index);
    // Method to set a value in a memory cell
    void setCell(int index, string value);
    // Method to get a value from a memory cell
    string getCell(int index);
    void clear();
signals:
    void MemoryUpdated();
};

// Register class
class Register : public QWidget {
    Q_OBJECT

    int memory[16]; // Array to store register values
    int size = 16; // Size of the register
public:
    // Constructor to initialize the register
    Register();
    // Method to set a value in a register cell
    void setCell(int index, int value);
    // Method to get a value from a register cell
    int getCell(int index);

    void clear();
signals:
    void registerUpdated();
};

// ALU class to handle arithmetic and logic operations
class ALU {
    // Method to convert a decimal integer to a floating point binary string
    string cnvrtToFloatingPoint(double);
    // Method to convert a binary string to a decimal integer in two's complement form
    int cnvrtTwosComplement(string);
public:
    // Method to check if a string is a valid hexadecimal number
    bool isValid(string);
    // Method to convert a hexadecimal string to a decimal integer
    static int hexToDec(string);
    // Method to convert a decimal integer to a hexadecimal string
    static string decToHex(int);
    // Method to convert a binary string to decimal integer
    static int binToDec(string);
    // Method to convert a decimal integer to a binary string
    static string decToBin(int, int = 8);
    // Method to sum
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
    void halt(Register& reg, Memory& mem, int PC, string IR);


};

// CPU class to handle the execution of instructions
class CPU : public QWidget {
    Q_OBJECT
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
    string getIR();
    int getPC();
    // Method to get the register object
    Register& getRegister();
    // Method to run the next step of the program
    void runNextStep(Memory&);

    void clear();
signals:
    void CPUupdated();
};

// Class to represent the machine with memory and CPU
class Machine {
    Memory mem; // Memory object to handle memory operations
    CPU cpu; // CPU object to handle instruction execution
public:
    // Default constructor
    Machine();
    // Method to get the CPU object
    CPU& getCPU();
    // Method to get the Register object
    Register& getRegister();
    // Method to get the Memory object
    Memory& getMemory();

    void clear();
};

#endif //VOLEMACHINESIM_VOLEMACHINESIM_H
