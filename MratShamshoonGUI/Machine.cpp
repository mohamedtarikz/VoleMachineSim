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
    playing = false;
}

void Machine::reset(){
    cpu.clear();
    if(mem.getCell(0)!="00" || mem.getCell(1)!="00"){
        mem.setCell(1,"00");
        mem.setCell(0,"00");
    }
    playing = false;
}

void Machine::play(int speed){
    playing = !playing;

    if(playing){
        int delay = 500;
        if(speed == 1){
            delay = 1500;
        } else if(speed == 2){
            delay = 3000;
        } else if (speed == 3) {
            delay = 0;
        }
        while(playing && cpu.getPC() <= 254){
            cpu.runNextStep(mem);

            QEventLoop loopHandle;
            QTimer::singleShot(delay, &loopHandle, &QEventLoop::quit);
            loopHandle.exec();
        }
        playing = false;
    }
}

void Machine::stop(){
    playing = false;
}

bool Machine::isPlaying(){
    return playing;
}
