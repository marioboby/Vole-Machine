//
// Created by mario saber on 10/24/2024.
//

#include "Machine.h"

// CPU Implementation
CPU::CPU() : programCounter(0), registerSet(new Register()), alu(new ALU()), cu(new CU()) {}

CPU::~CPU() {
    delete registerSet;
    delete alu;
    delete cu;
}

void CPU::runNextStep(Memory& memory) {
    fetch(memory);
    decode();
    vector<int> operands;
    execute(*registerSet, memory, operands);
}

void CPU::fetch(Memory& memory) {
    instructionRegister = memory.getCell(programCounter);
    programCounter++;
}

void CPU::decode() {
    // Decode instruction logic here
}

void CPU::execute(Register& reg, Memory& mem, std::vector<int>& operands) {
    // Execute logic here using ALU and CU
}

// Machine Implementation
Machine::Machine() : processor(new CPU()), memory(new Memory()) {}

Machine::~Machine() {
    delete processor;
    delete memory;
}

void Machine::loadProgramFile() {
    // Load program from a file into memory
}

void Machine::outputState() {
    // Output CPU and memory state
}

// MainUI Implementation
void MainUI::getFileOrInstructions() {
    // Logic to choose between file and manual instructions
}

void MainUI::displayMenu() {
    // Display user menu
}

void MainUI::inputFileName() {
    // Take file name input
}

void MainUI::inputInstruction() {
    // Take manual instruction input
}

char MainUI::inputChoice() {
    // Return user's choice
    return ' ';
}

Memory::Memory() {
    for (int i = 0; i < size; i++) {
        memory[i] = "00";
    }
}

// Memory Implementation
string Memory::getCell(int address) const {
    return memory[address];
}

void Memory::setCell(int address, const string& value) {
    memory[address] = value;
}

void Memory::clearMemory(int address) {
    memory[address] = "00";
}

void Memory::clear() {
    for (int i = 0; i < memory->size(); ++i) {
        memory[i] = "00";
    };
}

// Register Implementation
int Register::getCell(int index) const {
    return memory[index];
}

void Register::setCell(int index, int value) {
    memory[index] = value;
}

void Register::clearRegister(int index) {
    memory[index] = 00;
}

// CU Implementation
void CU::load(int idxReg, int intMem, Register& reg, Memory& mem) {
    reg.setCell(idxReg, std::stoi(mem.getCell(intMem)));
}

void CU::load(int idxReg, int valInt, Register& reg) {
    reg.setCell(idxReg, valInt);
}

void CU::store(int idxReg, int idxMem, Register& reg, Memory& mem) {
    mem.setCell(idxMem, std::to_string(reg.getCell(idxReg)));
}

void CU::move(int idx1, int idx2, Register& reg) {
    reg.setCell(idx1, reg.getCell(idx2));
}

void CU::jump(int idxMem, int idxMem2, Register& reg, int PC) {
    // Jump logic here
}

void CU::halt() {
    // Halt the system
}

// ALU Implementation
string ALU::hexToDec() {
    // Hex to Decimal conversion logic
    return "";
}

string ALU::decToHex() {
    // Decimal to Hex conversion logic
    return "";
}

bool ALU::isValid(const std::string& str) {
    // Check if string is a valid hex or dec number
    return true;
}

void ALU::add(int idx1, int idx2, int idx3, Register& reg) {
    reg.setCell(idx3, reg.getCell(idx1) + reg.getCell(idx2));
}
