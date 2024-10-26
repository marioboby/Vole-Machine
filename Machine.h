#ifndef MACHINE_H
#define MACHINE_H

#include <string>
#include <vector>

class ALU;
class CU;
class Memory;
class Register;

class CPU {
public:
    int programCounter;
    std::string instructionRegister;
    Register* registerSet;
    ALU* alu;
    CU* cu;

    CPU();
    void runNextStep(Memory&);
    void fetch(Memory&);
    void decode();
    void execute(Register&, Memory&, std::vector<int>&);
};

class Machine {
public:
    CPU* processor;
    Memory* memory;

    Machine();
    void loadProgramFile();
    void outputState();
};

class MainUI {
public:
    Machine* machine;
    bool enterFileOrInstructions;

    void getFileOrInstructions();
    void displayMenu();
    void inputFileName();
    void inputInstruction();
    char inputChoice();
};

class Memory {
public:
    string memory[256];
    int size = 256;

    Memory();

    string getCell(int address) const;
    void setCell(int address, const std::string& value);
    void clearMemory (int address);
    void clear ();
};

class Register {
public:
    int memory [16] = {0};
    int size = 16;

    int getCell(int index) const;
    void setCell(int index, int value);
    void clearRegister (int address);
};

class CU {
public:
    void load(int idxReg, int intMem, Register& reg, Memory& mem);
    void load(int idxReg, int valInt, Register& reg);
    void store(int idxReg, int idxMem, Register& reg, Memory& mem);
    void move(int idx1, int idx2, Register& reg);
    void jump(int idxMem, int idxMem2, Register& reg, int PC);
    void halt();
};

class ALU {
public:
    std::string hexToDec();
    std::string decToHex();
    bool isValid(const std::string& str);
    void add(int idx1, int idx2, int idx3, Register& reg);
};

#endif // MACHINE_H
