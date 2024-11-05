#include <algorithm>

#include "Machine.h"
using namespace std;

// Main function
int main() {
    Machine machine;
    Memory* memory = machine.memory;

    int choice;
    do {
        machine.displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: { // Load Program from File
                machine.loadProgramFile();
                break;
            }
            case 2: { // Execute Next Step
                machine.processor->cu->isRunning;
                machine.processor->runNextStep(machine.memory);
                machine.outputSubState();

                break;
            }
            case 3: { // Display Current State
                machine.outputState();
                break;
            }
            case 4: { // Run Full Program
                // Run the program
                while (machine.processor->cu->isRunning) {
                    machine.processor->runNextStep(memory);
                }
                break;
            }
            case 5: { // Run Full Program
                cout << "Enter new PC: ";
                int counter ;
                cin >> counter ;
                machine.processor->programCounter = counter;
                break;
            }
            case 6: { // Exit
                cout << "Exiting program.\n";
                break;
            }
            default:
                cout << "Invalid choice. Please select an option from the menu.\n";
            break;
        }
    } while (choice != 6);

    return 0;
}