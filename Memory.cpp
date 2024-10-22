#include "VoleMachineSim.h"

Memory::Memory(fstream& file){
    string byte;
    int i = 0;
    while(i < 256 && file >> byte){
        if(byte.size() != 4)
            throw invalid_argument("Invalid byte size");
        memory[i] = byte[0] + byte[1];
        memory[i+1] = byte[2] + byte[3];
        i+=2;
    }
}

Memory::Memory(vector<string> instructions){
    for(int i = 0; i < (size/2) && i < instructions.size(); i++){
        if(instructions[i].size() != 4)
            throw invalid_argument("Invalid byte size");
        memory[i] = instructions[i][0] + instructions[i][1];
        memory[i+1] = instructions[i][2] + instructions[i][3];
    }
}

void Memory::setCell(int index, string value){
    if(value.size() != 2)
        throw invalid_argument("Invalid byte size");
    assert(index >= 0 && index < size);
    memory[index] = value;
}

string Memory::getCell(int index){
    return memory[index];
}