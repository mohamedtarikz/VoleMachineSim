#include "VoleMachine.h"
#include <QEventLoop>
#include <QTimer>

// Constructor to initialize the Machine with default memory and CPU
Machine::Machine() : mem(), cpu() {playing = false;}

// Method to get a reference to the CPU
CPU& Machine::getCPU() {
    return cpu;
}

// Method to get a reference to the Register
Register& Machine::getRegister() {
    return cpu.getRegister();
}

// Method to get a reference to the Memory
Memory& Machine::getMemory() {
    return mem;
}

void Machine::clear(){
    cpu.clear();
    mem.clear();
}

void Machine::reset(){
    cpu.clear();
}

void Machine::play(int speed){
    playing = !playing;

    if(playing){
        int delay = 0;
        if(speed == 1){
            delay = 1500;
        } else if(speed == 2){
            delay = 3000;
        }
        while(playing && cpu.getPC() <= 254){
            cpu.runNextStep(mem);

            QEventLoop loopHandle;
            QTimer::singleShot(delay, &loopHandle, &QEventLoop::quit);
            loopHandle.exec();
        }
    }
}
