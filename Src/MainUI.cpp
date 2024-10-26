#include "VoleMachineSim.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

int GetInputChoice(const string& prompt, int gratest_choice) {
    int input;
    string line;
    while (true) {
        cout << prompt;
        if (getline(cin, line) && !line.empty()) {
            try {
                input = stoi(line);
                if (input > gratest_choice) {
                    cout << "Please enter a valid Choice.";
                } else {
                    return input;
                }
            } catch (const invalid_argument&) {
                cout << "Invalid input. Please enter a number." << endl;
            }
        } else {
            cout << "Empty input. Please try again." << endl;
        }
    }
}

string getFileName() {
    cout << "Please enter the file name including extension if it's in the same directory, or enter the absolute path: ";
    string file_name;
    getline(cin, file_name);
    return file_name;
}

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

vector<string> MainUI::loadinstruction() {
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
        if (counter == 127) {
            cout << "You have reached the maximum number of instructions." << endl;
            instructions.push_back("C000");
            break;
        }
    }
    return instructions;
}

void MainUI::outputState(Register& mainRegister, Memory& mainMemory) {
    cout << "Current state of Registers and Memory:" << endl;
    cout <<"Registers" << setw(2) << "|" << "Memory" << endl;
    for (int i = 0; i < 16; ++i) {
        cout <<ALU::decToHex(mainRegister.getCell(i)) << setw(10) << "|";
        for (int j = 0; j < 16; j++) {
            cout << mainMemory.getCell(i*16+j) << " ";
        }
        cout << endl;
    }
};

void MainUI::handleChoiceOutput(int choice, Machine& machine) {
    switch (choice) {
    case 1:
        while (machine.getCPU().getPC() < 256) {
            machine.getCPU().runNextStep(machine.getMemory());
        }
        break;
    case 2:
        machine.getCPU().runNextStep(machine.getMemory());
        break;
    case 3:
        outputState(machine.getRegister(), machine.getMemory());
        break;
    case 4:
        exit(0);
    default:
        cout << "Invalid choice. Please try again." << endl;
        break;
    }
};

int MainUI::DisplayOuputMenu(Machine& machine) {
    while(machine.getCPU().getPC() < 256){
        cout << "1- Play all Instructions." << endl;
        cout << "2- Play one step." << endl;
        cout << "3- Show state." << endl;
        cout << "4- Exit." << endl;
        cout << "Please enter a choice: ";
        choice = GetInputChoice("Please Enter your Choice: ", 4);
        handleChoiceOutput(choice, machine);
    }
};

void MainUI::handleChoiceOperation(int choice, Machine& machine) {
    if (choice == 1) {
        fstream file;
        file = LoadFile();
        machine.getMemory().loadMemory(file);
        DisplayOuputMenu(machine);
        return;
    } else if (choice == 2) {
        vector<string> instructions = loadinstruction();
        machine.getMemory().loadMemory(instructions);
        DisplayOuputMenu(machine);
        return;
    }else if(choice == 3){
        exit(0);
    } else {
        cout << "Invalid choice. Please try again." << endl;
        return;
    }
};

int MainUI::DisplayOperationMenu(Machine& machine) {
    while (true) {
        cout << "1- Operation using a file." << endl;
        cout << "2- Manual input operation." << endl;
        cout << "3- Exit." << endl;
        cout << "Please enter a choice: ";
        choice = GetInputChoice("Please Enter your Choice: ", 3);
        handleChoiceOperation(choice, machine);
    }
    return choice;
};