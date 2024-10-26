#include "Src/VoleMachineSim.h"
#include <iostream>

using namespace std;

int main () {
    MainUI ui;
    Machine machine;
    cout << "Welcome to the VOLE machine simulatour." << endl;
    ui.DisplayOperationMenu(machine);
    return 0;
}