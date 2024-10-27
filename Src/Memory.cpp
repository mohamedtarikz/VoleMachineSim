#include "VoleMachineSim.h"
#include <algorithm>

Memory::Memory(){
    int i = 0;
    while(i < 256){
        memory[i] = "00";
        i++;
    }
}

Memory::Memory(fstream& file){
    string byte;
    int i = 0;
    while(i < 256 && file >> byte){
        if(byte.size() != 4)
            throw invalid_argument("Invalid byte size");
        transform(byte.begin(), byte.end(), byte.begin(), ::toupper);
        string tmp;
        tmp += byte[0];
        tmp += byte[1];
        memory[i] = tmp;
        tmp = "";
        tmp += byte[2];
        tmp += byte[3];
        memory[i+1] = tmp;
        i+=2;
    }
}

Memory::Memory(vector<string> instructions){
    for(int i = 0; i < (size/2) && i < instructions.size(); i++){
        if(instructions[i].size() != 4)
            throw invalid_argument("Invalid byte size");
        string tmp;
        tmp += instructions[i][0];
        tmp += instructions[i][1];
        memory[i*2] = tmp;
        tmp = "";
        tmp += instructions[i][2];
        tmp += instructions[i][3];
        memory[i*2+1] = tmp;
    }
}

void Memory::loadMemory(fstream &file) {
    string byte;
    int i = 0;
    while(i < 256 && file >> byte){
        if(byte.size() != 4)
            throw invalid_argument("Invalid byte size");
        transform(byte.begin(), byte.end(), byte.begin(), ::toupper);
        string tmp;
        tmp += byte[0];
        tmp += byte[1];
        memory[i] = tmp;
        tmp = "";
        tmp += byte[2];
        tmp += byte[3];
        memory[i+1] = tmp;
        i+=2;
    }
}

void Memory::loadMemory(vector<string> instructions) {
    for(int i = 0; i < (size/2) && i < instructions.size(); i++){
        if(instructions[i].size() != 4)
            throw invalid_argument("Invalid byte size");
        string tmp;
        tmp += instructions[i][0];
        tmp += instructions[i][1];
        memory[i*2] = tmp;
        tmp = "";
        tmp += instructions[i][2];
        tmp += instructions[i][3];
        memory[i*2+1] = tmp;
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