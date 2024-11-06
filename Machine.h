#ifndef MACHINE_H
#define MACHINE_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include  <fstream>
#include <bitset>
#include <cstdint>
#include <cmath>
using namespace std;

class ALU;
class CU;
class Memory;
class Register;

class CPU {
public:
    int programCounter;
    string instructionRegister;
    Register* registerSet;
    ALU* alu;
    CU* cu;

    CPU();
    ~CPU();
    void runNextStep(Memory*);
    void fetch(Memory*);
    vector<int> decode();
    void execute(Register*, Memory*, vector<int>&);
};

class Machine {
public:
    CPU* processor;
    Memory* memory;

    Machine();
    ~Machine();
    void displayMenu();
    void loadProgramFile();
    void outputState();
    void outputSubState();
};

class Memory {
public:
    string memory[256];
    int size = 256;

    Memory();

    string getCell(int address) const;
    void setCell(int address, const std::string& value);
    void clearMemory (int address);
    void clearAllMemory ();
};

class Register {
public:
    int memory [16] = {0};
    int size = 16;

    int getCell(int index) const;
    void setCell(int index, int value);
    void clearRegister (int address);
    void clearAll();
};

class CU {
public:
    bool isRunning = true ;
    void load(int idxReg, int intMem, Register* reg, Memory* mem);
    void load(int idxReg, int valInt, Register* reg);
    void store(int idxReg, int idxMem, Register* reg, Memory* mem);
    void storeToScreen(int regIndex, Register* reg, Memory* mem, ALU* alu);
    void move(int idx1, int idx2, Register* reg);
    void jump(int regIndex, int targetAddress, Register* reg, int& programCounter);
    void jumpComplement(int regIndex, int targetAddress, Register* reg, int& programCounter);
    void halt(CU* cu);
};

class ALU {
public:
    string hexToBinary(const string& hex);
    float binaryToIEEE754(const string& binary);
    string decimalToHexIEEE754(float decimal);
    string hexToDec(string& hexStr);
    string decToHex(int decimal);
    bool isValid(const string& str);
    void add(int idx1, int idx2, int idx3, Register* reg);
    void addFloatingPoint(int destRegIndex, int regIndex1, int regIndex2, Register* reg);
    void orop(int r, int s,int t, Register* reg);
    void andop(int r, int s,int t, Register* reg);
    void xorop(int r, int s,int t, Register* reg);
    void rotateRight(int regIdx, int rotations, Register* reg, ALU* alu);
    string bitsetToHex(const bitset<8>& bits);
};

#endif //MACHINE_H
