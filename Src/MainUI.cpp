#include "VoleMachineSim.h"
#include <iostream>

int GetInputChoice(const string& prompt , int gratest_choice) {
    int input;
    string line;
    while (true) {
        cout << prompt;
        if (getline(cin, line) && !line.empty()) {
            try {
                input = stoi(line);
                if (input > gratest_choice) {
                    cout << "Please enter a valid Choice.";
                }else {
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

void LoadFile(fstream& dataFile) {
    string Filename;
    while (true) {
        Filename = getFileName(); // Get file name from user
        dataFile.open(Filename, ios::in); // Attempt to open the file
        if (dataFile.fail()) {
            cout << "Error opening file, please try again." << endl;
        } else {
            break; // File opened successfully
        }
    }
}

void MainUI::loadinstruction(vector<string>& instructions){
    string instruction;
    cout<<"Please enter the instructions in hex format. Enter C000 as a last instruction."<<endl;
    while(true){
        cin>>instruction;
        if(instruction=="C000")
            break;
        instructions.push_back(instruction);
    }
    Memory memory(instructions);
}

void MainUI::DisplayOuputMenu() {
    cout << "1- Display Memory." << endl;
    cout << "2- Display Register." << endl;
    cout << "3- Display Booth." << endl;
    cout << "4- Exit." << endl;
    cout << "Please enter a choice: ";
    choice = GetInputChoice("Please Enter your Choice: ",4);

}

void MainUI::DisplayoperationMenu() {
    cout << "1- Operation using a file." << endl;
    cout << "2- Manual input operation." << endl;
    cout << "3- Exit." << endl;
    cout << "Please enter a choice: ";
    choice = GetInputChoice("Please Enter your Choice: ",3);
    if (choice == 1) {
        cout << "Operation using a file." << endl;
        LoadFile(dataFile);
        Memory memory(dataFile);
    }
    else if (choice == 2) {
        cout << "Manual input operation." << endl;
        vector<string> instructions;
        loadinstruction(instructions);
    }
    else if (choice == 3) {
        cout << "Exiting the program." << endl;
        return;
    }
};
void MainUI::PrintRegister(Register& mainRegister) {
    cout << "Current state of registers:" << endl;
    for (int i = 0; i < 16; ++i) {
        cout << "Register[" << i << "] = " <<  mainRegister.getCell(i)<< endl;
    }
};