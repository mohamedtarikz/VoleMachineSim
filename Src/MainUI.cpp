#include "VoleMachineSim.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>

// Function to get user input choice with validation
int GetInputChoice(const string& prompt, int gratest_choice) {
    int input;
    string line;
    while (true) {
        cout << prompt;
        getline(cin, line);
        if (!line.empty()) {
            try {
                input = stoi(line);
                if (input > gratest_choice) {
                    cout << "Please enter a valid Choice.";
                } else {
                    return input;
                }
            } catch (invalid_argument& e) {
                cout << "Invalid input. Please enter a number." << endl;
            }
        } else {
            cout << "Empty input. Please try again." << endl;
        }
    }
}

// Function to get the file name from the user
string getFileName() {
    cout << "Please enter the file name including extension if it's in the same directory, or enter the absolute path: ";
    string file_name;
    getline(cin, file_name);
    return file_name;
}

// Method to load a file
fstream MainUI::LoadFile() {
    string Filename;
    fstream programFile(Filename, ios::in);
    while (true) {
        Filename = getFileName(); // Get file name from user
        programFile.open(Filename, ios::in); // Attempt to open the file
        if (programFile.fail()) {
            cout << "Error opening file, please try again." << endl;
        } else {
            break; // File opened successfully
        }
    }
    return programFile;
}

// Method to load instructions manually
vector<string> MainUI::loadInstruction(int start) {
    vector<string> instructions;
    string instruction;
    int counter = 0;
    cout << "Please enter the instructions in hex format. Enter C000 as a last instruction." << endl;
    while (true) {
        cin >> instruction;
        transform(instruction.begin(), instruction.end(), instruction.begin(), ::toupper);
        instructions.push_back(instruction);
        if (instruction == "C000")
            break;
        counter++;
        if (counter == 127 - ceil(start/2.0)) {
            cout << "You have reached the maximum number of instructions." << endl;
            instructions.push_back("C000");
            break;
        }
    }
    return instructions;
}

// Static method to output the state of the register and memory
void MainUI::outputState(Register& mainRegister, Memory& mainMemory, int PC, string IR) {
    cout << "Current state of Registers and Memory:" << endl;
    cout <<"Registers" << setw(3) << "|" << "Memory" << endl;
    for (int i = 0; i < 16; ++i) {
        cout <<ALU::decToHex(mainRegister.getCell(i)) << setw(10) << "|";
        for (int j = 0; j < 16; j++) {
            cout << mainMemory.getCell(i*16+j) << " ";
        }
        cout << endl;
    }
    cout<<"PC: " << PC << endl;
    cout<<"IR: " << IR << endl;
}

// Method to handle the output choice
void MainUI::handleChoiceOutput(int choice, Machine& machine) {
    switch (choice) {
        case 1:
            while (true) {
                machine.getCPU().runNextStep(machine.getMemory());
            }
            break;
        case 2:
            machine.getCPU().runNextStep(machine.getMemory());
            break;
        case 3:
            outputState(machine.getRegister(), machine.getMemory(), machine.getCPU().getPC(), machine.getCPU().getIR());
            break;
        case 4:
            exit(0);
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
    }
}

// Method to display the output menu
int MainUI::DisplayOutputMenu(Machine& machine) {
    while(machine.getCPU().getPC() < 255){
        cout << "1- Play all Instructions." << endl;
        cout << "2- Play one step." << endl;
        cout << "3- Show state." << endl;
        cout << "4- Exit." << endl;
        choice = GetInputChoice("Please Enter your Choice: ", 4);
        handleChoiceOutput(choice, machine);
    }
    return choice;
}

// Method to handle the operation choice
void MainUI::handleChoiceOperation(int choice, Machine& machine) {
    if (choice == 1) {
        cout << "Enter your desired start index in the memory (2 - 254): ";
        string strStart;
        int start;
        bool ok = false;
        getline(cin, strStart);
        while(!ok) {
            ok = true;
            for (int i = 0; i < strStart.size(); ++i) {
                if(!isdigit(strStart[i])) {
                    ok = false;
                    cout << "Invalid input. Please enter a decimal number." << endl;
                    break;
                }
            }
            if (ok) {
                start = max(2, min(254, stoi(strStart)));
            }
        }
        fstream file;
        file = LoadFile();
        machine.getMemory().loadMemory(file, start);
        DisplayOutputMenu(machine);
        return;
    } else if (choice == 2) {
        cout << "Enter your desired start index in the memory (2 - 254): ";
        string strStart;
        int start;
        bool ok = false;
        getline(cin, strStart);
        while(!ok) {
            ok = true;
            for (int i = 0; i < strStart.size(); ++i) {
                if(!isdigit(strStart[i])) {
                    ok = false;
                    cout << "Invalid input. Please enter a decimal number." << endl;
                    break;
                }
            }
            if (ok) {
                start = max(2, min(254, stoi(strStart)));
            }
        }
        vector<string> instructions = loadInstruction();
        machine.getMemory().loadMemory(instructions, start);
        DisplayOutputMenu(machine);
        return;
    }else if(choice == 3){
        exit(0);
    } else {
        cout << "Invalid choice. Please try again." << endl;
        return;
    }
}

// Method to display the operation menu
int MainUI::DisplayOperationMenu(Machine& machine) {
    while (true) {
        cout << "1- Operation using a file." << endl;
        cout << "2- Manual input operation." << endl;
        cout << "3- Exit." << endl;
        choice = GetInputChoice("Please Enter your Choice: ", 3);
        handleChoiceOperation(choice, machine);
    }
    return choice;
}