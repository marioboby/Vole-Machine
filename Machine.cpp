//
// Created by mario saber on 10/24/2024.
//

#include "Machine.h"


// CPU Implementation
CPU::CPU() : programCounter(16), registerSet(new Register()), alu(new ALU()), cu(new CU()) {}

CPU::~CPU() {
    delete registerSet;
    delete alu;
    delete cu;
}

void CPU::runNextStep(Memory* memory) {
    fetch(memory);
    vector<int> operands = decode();
    execute(registerSet, memory, operands);
}

void CPU::fetch(Memory* memory) {
    string part1 = memory->getCell(programCounter);
    string part2 = memory->getCell(programCounter + 1);
    instructionRegister = part1 + part2;
    programCounter += 2;
}

vector<int> CPU::decode() {
    vector<int> decoded;
    // Assuming each instruction is a 2-character hex code
    for (size_t i = 0; i < instructionRegister.size(); i += 1) {
        string part = instructionRegister.substr(i, 1);
        int value = stoi(part, nullptr, 16);  // Convert hex to integer
        decoded.push_back(value);
    }
    return decoded;
}

void CPU::execute(Register* reg, Memory* mem, vector<int>& operands) {
    int opcode = operands[0];
    switch (opcode) {
        case 0x1: // Load
            cu->load(operands[1], 16 *  operands[2] + operands [3] , reg, mem);
            break;
        case 0x2: // Load Immediate
            cu->load(operands[1], 16 *  operands[2] + operands [3], reg);
            break;
        case 0x3: // Store
            if (operands[2] == 0x0 && operands[3] == 0x0 ) {
                cu->storeToScreen(operands[1], reg, mem, alu); // Special case for store to screen
            } else {
                cu->store(operands[1], operands[2] * 16 + operands[3], reg, mem);
            }
            break;
        case 0x4: // Move
            cu->move(operands[3], operands[2], reg);
            break;
        case 0x5: // Add Two’s Complement
            alu->add(operands[1], operands[2], operands[3], reg);
            break;
        case 0x6: // Add Floating-Point
            alu->addFloatingPoint(operands[1], operands[2], operands[3], reg);
            break;
        case 0x7: // Bitwise OR
            alu->orop(operands[1], operands[2], operands[3], reg);
            break;
        case 0x8: // Bitwise AND
            alu->andop(operands[1], operands[2], operands[3], reg);
            break;
        case 0x9: // Bitwise XOR
            alu->xorop(operands[1], operands[2], operands[3], reg);
            break;
        case 0xA: // Rotate
            alu->rotateRight(operands[1], operands[3], reg, alu);
        break;
        case 0xB: // Conditional Jump
            cu->jump(operands[1], operands[2] * 16 + operands[3], reg, programCounter);
            break;
        case 0xC: // Halt
            cu->halt(cu);
            break;
        case 0xD: // Halt
            cu->jumpComplement(operands[1], operands[2] * 16 + operands[3], reg, programCounter);
            break;
        default:
            cerr << "Unknown opcode: " << opcode << endl;
    }
}

// Machine Implementation
Machine::Machine() : processor(new CPU()), memory(new Memory()) {}

Machine::~Machine() {
    delete processor;
    delete memory;
}

void Machine::displayMenu() {
    cout << "\n--- Machine Simulator Menu ---\n";
    cout << "1. Load Program from File\n";
    cout << "2. Execute Next Step\n";
    cout << "3. Display Current State\n";
    cout << "4. Run Full Program\n";
    cout << "5. Change PC\n";
    cout << "6. Clean Memories\n";
    cout << "7. Clean Registers\n";
    cout << "8. Exit\n";
    cout << "Choose an option: ";
}

// Function to load a program from a file into memory
void Machine::loadProgramFile() {
    string filename;
    cout << "Enter the instruction file name: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;

    }

    string instruction;
    int address = processor->programCounter;

    // Read instructions from file and load them into memory
    while (getline(file, instruction)) {
        if (instruction.size() != 4) {
            cerr << "Invalid instruction format in file. Each line should contain a 4-character hex code.\n";

        }

        // Split instruction into 2-character parts
        string opcode_operand1 = instruction.substr(0, 2); // First 2 characters
        string operand2_operand3 = instruction.substr(2, 2); // Last 2 characters

        // Store instruction parts in consecutive memory cells
        memory->setCell(address++, opcode_operand1);
        memory->setCell(address++, operand2_operand3);
    }

    file.close();
    cout << "\nProgram Loaded Successfully\n";
}

void Machine::outputSubState() {
    cout << "Program Counter: " << dec << processor->programCounter << endl;
    cout << "Instruction Register: " << processor->instructionRegister << endl;
    cout << endl;
}

// Function to display the state of the CPU and memory
void Machine::outputState() {
    cout << "Program Counter: " << dec <<  processor->programCounter << endl;
    cout << "Instruction Register: " << processor->instructionRegister << endl;

    cout << "\nRegister State:" << endl;
    for (int i = 0; i < processor->registerSet->size; ++i) {
        cout << "R" << hex << uppercase << i << ": " << processor->registerSet->getCell(i) << "\n";
    }

    cout << "\n\nMemory State:" << endl;
    for (int i = 0; i < memory->size ; ++i) {
        if (i % 16 == 0) cout << endl; // New line every 16 addresses
        cout << setw(2) << hex << uppercase << i << ": " << memory->getCell(i) << " ";
    }
    cout << endl;
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
    if (address >= 0 && address < size) {
        memory[address] = value;
    }
}

void Memory::clearMemory(int address) {
    setCell(address,"00");
}

void Memory::clearAllMemory() {
    for (int i = 0; i < size; ++i) {
        setCell(i,"00");
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

void Register::clearAll() {
    for (int i = 0; i < size; ++i) {
        memory[i] = 0;
    };
}

// CU Implementation
void CU::load(int idxReg, int intMem, Register* reg, Memory* mem) {
    reg->setCell(idxReg, stoi(mem->getCell(intMem)));
}

void CU::load(int idxReg, int valInt, Register* reg) {
    reg->setCell(idxReg, valInt);
}

void CU::store(int idxReg, int idxMem, Register* reg, Memory* mem) {
    mem->setCell(idxMem, to_string(reg->getCell(idxReg)));
}

void CU::storeToScreen(int regIndex, Register* reg, Memory* mem, ALU* alu) {
    string value = alu->decToHex(reg->getCell(regIndex)); // Get value from register R
    mem->setCell(0, value);
    cout << "Memory [00]= " << uppercase << mem->getCell(0) << endl; // Store to memory address 00
}

void CU::move(int idx1, int idx2, Register* reg) {
    reg->setCell(idx1, reg->getCell(idx2));
}

void CU::jump(int regIndex, int targetAddress, Register* reg, int& programCounter) {
    if (reg->getCell(regIndex) == reg->getCell(0)) { // Compare R with register 0
        programCounter = targetAddress; // Set PC to XY if equal
    }
}

void CU::jumpComplement(int regIndex, int targetAddress, Register* reg, int& programCounter) {
    uint8_t x = reg->getCell(regIndex);
    //cout << "x: " <<  (int)int8_t(x) << endl; ;
    if ( int8_t(x) > reg->getCell(0)) { // Compare R with register 0
        programCounter = targetAddress; // Set PC to XY if greater
    }
}

void CU::halt(CU* cu) {
    cout << "Programm excecuted successfully!\n" ;
    cu->isRunning = false;
}

// ALU Implementation
string ALU::hexToDec(string& hexStr) {
    int value;
    stringstream ss;
    ss << hex << hexStr;
    ss >> value;
    return to_string(value);
}



string ALU::decToHex(int decimal) {
    stringstream ss;
    ss << hex << decimal;
    return ss.str();
}


bool ALU::isValid(const string& str) {
    for (char c : str) {
        if (!isxdigit(c)) return false;
    }
    return true;
}

string ALU::hexToBinary(const string& hex) {
    int value = stoi(hex, nullptr, 16);
    bitset<8> binary(value);
    cout << binary.to_string() << endl;
    return binary.to_string();
}

float ALU::binaryToIEEE754(const string& binary) {
    int sign = binary[0] == '1' ? -1 : 1;
    int exponent = stoi(binary.substr(1, 3), nullptr, 2) - 4;
    float mantissa = 0.0;
    if (exponent >= 0) {
        mantissa += (binary[4] - '0') * 0.5f +
                    (binary[5] - '0') * 0.25f +
                    (binary[6] - '0') * 0.125f +
                    (binary[7] - '0') * 0.0625f;
    } else {
        mantissa = (binary[4] - '0') * 0.5f +
                   (binary[5] - '0') * 0.25f +
                   (binary[6] - '0') * 0.125f +
                   (binary[7] - '0') * 0.0625f;
    }
    return sign * mantissa * pow(2, exponent);
}

string ALU::decimalToHexIEEE754(float decimal) {
    int sign = decimal < 0 ? 1 : 0;
    if (sign) {
        decimal = -decimal;
    }
    if (decimal == 0.0f) {
        return sign == 1 ? "80" : "00";
    }
    int shiftCount = 0;
    while (decimal >= 0.5f) {
        decimal /= 2.0f;
        shiftCount++;
    }
    while (decimal < 0.5f) {
        decimal *= 2.0f;
        shiftCount--;
    }
    int exponent = shiftCount + 4;
    if (exponent < 0 || exponent > 7) {
        throw overflow_error("The number is out of range for this representation.");
    }
    bitset<3> exponentBits(exponent);
    cout << "Exponent: " << exponent << " " << exponentBits << endl;
    int mantissa = static_cast<int>(decimal * 16);
    bitset<4> mantissaBits(mantissa);
    string binary = to_string(sign) + exponentBits.to_string() + mantissaBits.to_string();
    bitset<8> binaryBits(binary);
    cout << binaryBits << endl;
    stringstream hexStream;
    hexStream << hex << uppercase << setw(2) << setfill('0') << binaryBits.to_ulong();
    return hexStream.str();
}

void ALU::addFloatingPoint(int destRegIndex, int regIndex1, int regIndex2, Register* reg) {
    float value1 = binaryToIEEE754(hexToBinary(decToHex(reg->getCell(regIndex1))));
    float value2 = binaryToIEEE754(hexToBinary(decToHex(reg->getCell(regIndex2))));

    cout << value1 << endl;
    cout << value2 << endl;
    float result = value1 + value2;
    cout << result << endl;
    reg->setCell(destRegIndex, stoi(decimalToHexIEEE754(result), nullptr , 16));
}

void ALU::add(int idx1, int idx2, int idx3, Register* reg) {
    uint8_t x = reg->getCell(idx2);
    uint8_t y = reg->getCell(idx3);
    uint8_t r = x + y;
    reg->setCell(idx1, r);
}

void ALU::orop(int r, int s,int t, Register* reg) {
    int a = reg->getCell(s) | reg->getCell(t);
    reg->setCell(r, a);
}

void ALU::andop(int r, int s,int t, Register* reg) {
    int a = reg->getCell(s) & reg->getCell(t);
    reg->setCell(r, a);
}

void ALU::xorop(int r, int s,int t, Register* reg) {
    int a = reg->getCell(s) ^ reg->getCell(t);
    reg->setCell(r, a);
}

void ALU::rotateRight(int regIdx, int rotations, Register* reg, ALU* alu) {
    const int BITS = 8;
    int value = reg->getCell(regIdx);

    rotations = rotations % BITS;

    int sftrt = value >> rotations;
    int sftlft = value << (BITS - rotations);

    unsigned int rotated = (sftrt | sftlft);

    string final = bitsetToHex(bitset<8> (rotated));

    reg->setCell(regIdx,stoi(final, nullptr , 16));
}

string ALU::bitsetToHex(const bitset<8>& bits) {
    // Convert the bitset to an unsigned long integer
    unsigned long decimalValue = bits.to_ulong();

    // Convert the integer to a hex string
    stringstream hexStream;
    hexStream << hex << uppercase << setw(2) << setfill('0') << decimalValue;

    return hexStream.str();
}
