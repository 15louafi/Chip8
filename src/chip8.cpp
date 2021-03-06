#include <fstream>
#include <random>
#include <cstdint>
#include <cstring>
#include "chip8.hpp"

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;

uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8() {
    init();
}

// Initialise
void Chip8::init() {
    programCounter=START_ADDRESS;
    opcode = 0; // Reset op code
    index = 0; // Reset index
    stackPointer = 0; // Reset stack pointer

    // Reset timers
    delayTimer = 0;
    soundTimer = 0;

    // Clear the display
    for (int i = 0; i < 2048; ++i) {
        display[i] = 0;
    }

    // Clear the stack, keypad, and registers
    for (int i = 0; i < 16; ++i) {
        stack[i] = 0;
        keyPad[i] = 0;
        registers[i] = 0;
    }

    // Clear memory
    for (int i = 0; i < 4096; ++i) {
        memory[i] = 0;
    }

    // Load font set into memory
    for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
    {
        memory[FONTSET_START_ADDRESS+i] = fontset[i];
    }

    //random seed for RNG's process
    srand(time(NULL));
}

void Chip8::LoadROM(char const* filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate );
    if(file.is_open()) 
    {
        std::streampos size = file.tellg();
        file.seekg(0, std::ios::beg);
        char* buffer = new char[size];
        file.read (buffer,size);
        file.close();
        for (long i =0; i<size; ++i) {
            memory[START_ADDRESS+i] = buffer[i];
        }
        delete[] buffer;
    }
}

void Chip8::EmulateCycle() {
    opcode = memory[programCounter] << 8 | memory[programCounter + 1];  
	programCounter += 2;
	ExecuteOpcode();
	if (delayTimer > 0)
	{
		--delayTimer;
	}

	// Decrement the sound timer if it's been set
	if (soundTimer > 0)
	{
		--soundTimer;
	}
}

void Chip8::ExecuteOpcode() {
     switch(opcode & 0xF000){

        // 00E_
        case 0x0000:

            switch (opcode & 0x000F) {
                case 0x0000:
                    OP_00E0();
		    break;
                case 0x000E:
                    OP_00EE();
                    break;
                default:
                    printf("\nUnknown opcode: %.4X\n", opcode);
                    exit(3);
            }
            break;

        case 0x1000:
            OP_1NNN();
	    break;

        case 0x2000:
            OP_2NNN();
	    break;

        case 0x3000:
            OP_3XKK();
	    break;

        case 0x4000:
            OP_4XKK();
	    break;

        case 0x5000:
            OP_5XY0();
	    break;

        case 0x6000:
            OP_6XKK();
	    break;

        case 0x7000:
            OP_7XKK();
	    break;

        // 8XY_
        case 0x8000:
            switch (opcode & 0x000F) {

                case 0x0000:
                    OP_8XY0();
		    break;

                case 0x0001:
                    OP_8XY1();
		    break;

                case 0x0002:
                    OP_8XY2();
		    break;

                case 0x0003:
                    OP_8XY3();
		    break;

                case 0x0004:
                    OP_8XY4();
		    break;

                case 0x0005:
                    OP_8XY5();
		    break;

                case 0x0006:
                    OP_8XY6();
		    break;

                case 0x0007:
                    OP_8XY7();
		    break;

                case 0x000E:
                    OP_8XYE();
		    break;

                default:
                    printf("\nUnknown op code: %.4X\n", opcode);
                    exit(3);
            }
            break;

        case 0x9000:
            OP_9XY0();
	    break;

        case 0xA000:
            OP_ANNN();
	    break;

        case 0xB000:
            OP_BNNN();
	    break;

        case 0xC000:
            OP_CXKK();
	    break;

        case 0xD000:
            OP_DXYN();
	    break;

        // EX__
        case 0xE000:

            switch (opcode & 0x00FF) {
                case 0x009E:
                    OP_EX9E();
		    break;

                case 0x00A1:
                    OP_EXA1();
		    break;

                default:
                    printf("\nUnknown op code: %.4X\n", opcode);
                    exit(3);
            }
            break;

        // FX__
        case 0xF000:
            switch(opcode & 0x00FF)
            {
                case 0x0007:
                    OP_FX07();
		    break;

                case 0x000A:
                    OP_FX0A();
		    break;

                case 0x0015:
                    OP_FX15();
		    break;

                case 0x0018:
                    OP_FX18();
		    break;

                case 0x001E:
                    OP_FX1E();
		    break;

                case 0x0029:
                    OP_FX29();
		    break;

                case 0x0033:
                    OP_FX33();
		    break;

                case 0x0055:
                    OP_FX55();
		    break;

                case 0x0065:
                    OP_FX65();
		    break;

                default:
                    printf ("Unknown opcode [0xF000]: 0x%X\n", opcode);
            }
        break;

        default:
            printf("\nUnimplemented op code: %.4X\n", opcode);
            exit(3);
}
}

// 00E0 - Clear screen
void Chip8::OP_00E0()
{
	memset(display, 0, sizeof(display));
}

// 00EE - Return from subroutine
void Chip8::OP_00EE()
{
	--stackPointer;
	programCounter = stack[stackPointer];
}

// 1NNN - Jumps to address NNN
void Chip8::OP_1NNN()
{
    uint16_t counter = opcode & 0x0FFFu;
    programCounter = counter;
}

// 2NNN - Calls subroutine at NNN
void Chip8::OP_2NNN()
{
    uint16_t counter = opcode & 0x0FFFu;
    stack[stackPointer] = programCounter;
	++stackPointer;
	programCounter = counter;
}

// 3XNN - Skips the next instruction if VX equals NN.
void Chip8::OP_3XKK()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t value = (opcode & 0x00FFu);
    if(registers[Vx] == value) {
        programCounter+=2;
    }
}

// 4XKK - Skips the next instruction if VX does not equal KK.
void Chip8::OP_4XKK()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t value = (opcode & 0x00FFu);
    if(registers[Vx] != value) {
        programCounter+=2;
    }
}

// 5XY0 - Skips the next instruction if VX equals VY.
void Chip8::OP_5XY0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >>4u;
    if(registers[Vx] == registers[Vy]) {
        programCounter+=2;
    }
}

// 6XNN - Sets VX to NN.
void Chip8::OP_6XKK()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t value = (opcode & 0x00FFu);
    registers[Vx] = value;
}

// 7XNN - Adds NN to VX.
void Chip8::OP_7XKK()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t value = (opcode & 0x00FFu);
    registers[Vx] += value;
}

// 8XY0 - Sets VX to the value of VY.
void Chip8::OP_8XY0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    registers[Vx] = registers[Vy];
}

// 8XY1 - Sets VX to (VX OR VY).
void Chip8::OP_8XY1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    registers[Vx] |= registers[Vy];
}

// 8XY2 - Sets VX to (VX AND VY).
void Chip8::OP_8XY2()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    registers[Vx] &= registers[Vy];
}

// 8XY3 - Sets VX to (VX XOR VY).
void Chip8::OP_8XY3()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    registers[Vx] ^= registers[Vy];
}

// 8XY4 - Adds VY to VX. VF is set to 1 when there's a carry,
// and to 0 when there isn't.
void Chip8::OP_8XY4()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    uint16_t result = registers[Vx]+registers[Vy];
    if(result > 0x00FFu) {
        registers[0xF] = 1;
    }
    else {
        registers[0xF] = 0;
    }
    registers[Vx] = result & (0xFFu);
}

// 8XY5 - VY is subtracted from VX. VF is set to 0 when
// there's a borrow, and 1 when there isn't.
void Chip8::OP_8XY5()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    if (registers[Vx] > registers[Vy])
	{
		registers[0xF] = 1;
	}
	else
	{
		registers[0xF] = 0;
	}

	registers[Vx] -= registers[Vy];
}

// 0x8XY6 - Shifts VX right by one. VF is set to the value of
// the least significant bit of VX before the shift.
void Chip8::OP_8XY6()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    registers[0xF] = registers[Vx] & 0x1u;
    registers[Vx] >>= 1;
}

// 0x8XY7: Sets VX to VY minus VX. VF is set to 0 when there's
// a borrow, and 1 when there isn't.
void Chip8::OP_8XY7()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    if (registers[Vy] > registers[Vx])
	{
		registers[0xF] = 1;
	}
	else
	{
		registers[0xF] = 0;
	}
    registers[Vx] = registers[Vy] - registers[Vx];
}

// 0x8XYE: Shifts VX left by one. VF is set to the value of
// the most significant bit of VX before the shift.
void Chip8::OP_8XYE()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    registers[0xF] = (registers[Vx] & 0x80u) >> 7u;
    registers[Vx] <<= 1;
}

// 9XY0 - Skips the next instruction if VX doesn't equal VY.
void Chip8::OP_9XY0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    if (registers[Vy] != registers[Vx])
	{
		programCounter +=2;
	}
}

// ANNN - Sets I to the address NNN.
void Chip8::OP_ANNN()
{
    uint16_t value = (opcode & 0x0FFFu);
    index = value;
}

// BNNN - Jumps to the address NNN plus V0.
void Chip8::OP_BNNN()
{
    uint8_t value = (opcode & 0x0FFFu);
    programCounter = registers[0] + value;
}

// CXNN - Sets VX to a random number, masked by NN.
void Chip8::OP_CXKK()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t value = (opcode & 0x00FFu);
    registers[Vx] = (rand() % (0xFF + 1)) & value;
}

// DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8
// pixels and a height of N pixels.
// Each row of 8 pixels is read as bit-coded starting from memory
// location I;
// I value doesn't change after the execution of this instruction.
// VF is set to 1 if any screen pixels are flipped from set to unset
// when the sprite is drawn, and to 0 if that doesn't happen. 
void Chip8::OP_DXYN()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t height = opcode & 0x000Fu;

	// Wrap if going beyond screen boundaries
	uint8_t xPos = registers[Vx];
	uint8_t yPos = registers[Vy];

	registers[0xF] = 0;

	for (int yline = 0; yline < height; yline++)
            {
                uint8_t pixel = memory[index + yline];

                for(int xline = 0; xline < 8; xline++)
                {
                    if((pixel & (0x80 >> xline)) != 0)
                    {
                        if(display[(xPos + xline + ((yPos + yline) * 64))] == 1)
                        {
                            registers[0xF] = 1;
                        }
                        display[xPos + xline + ((yPos + yline) * 64)] ^= 1;
                    }
                }
            }
}

// EX9E - Skips the next instruction if the key stored
// in VX is pressed.
void Chip8::OP_EX9E()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    if(keyPad[registers[Vx]])
    {
        programCounter +=2;
    }
}

// EXA1 - Skips the next instruction if the key stored
// in VX isn't pressed.
void Chip8::OP_EXA1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    if(!keyPad[registers[Vx]])
    {
        programCounter +=2;
    }
}

// FX07 - Sets VX to the value of the delay timer
void Chip8::OP_FX07()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    registers[Vx] = delayTimer;
}

// FX0A - A key press is awaited, and then stored in VX
void Chip8::OP_FX0A()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    if (keyPad[0])
	{
		registers[Vx] = 0;
	}
	else if (keyPad[1])
	{
		registers[Vx] = 1;
	}
	else if (keyPad[2])
	{
		registers[Vx] = 2;
	}
	else if (keyPad[3])
	{
		registers[Vx] = 3;
	}
	else if (keyPad[4])
	{
		registers[Vx] = 4;
	}
	else if (keyPad[5])
	{
		registers[Vx] = 5;
	}
	else if (keyPad[6])
	{
		registers[Vx] = 6;
	}
	else if (keyPad[7])
	{
		registers[Vx] = 7;
	}
	else if (keyPad[8])
	{
		registers[Vx] = 8;
	}
	else if (keyPad[9])
	{
		registers[Vx] = 9;
	}
	else if (keyPad[10])
	{
		registers[Vx] = 10;
	}
	else if (keyPad[11])
	{
		registers[Vx] = 11;
	}
	else if (keyPad[12])
	{
		registers[Vx] = 12;
	}
	else if (keyPad[13])
	{
		registers[Vx] = 13;
	}
	else if (keyPad[14])
	{
		registers[Vx] = 14;
	}
	else if (keyPad[15])
	{
		registers[Vx] = 15;
	}
	else
	{
		programCounter -= 2;
	}
}

// FX15 - Sets the delay timer to VX
void Chip8::OP_FX15()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    delayTimer = registers[Vx];
}

// FX18 - Sets the sound timer to VX
void Chip8::OP_FX18()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    
    soundTimer = registers[Vx];
}

// FX1E - Adds VX to I
void Chip8::OP_FX1E()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    index += registers[Vx];
}

// FX29 - Sets I to the location of the sprite for the
// character in VX. Characters 0-F (in hexadecimal) are
// represented by a 4x5 font
void Chip8::OP_FX29()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t digit = registers[Vx];
	index = FONTSET_START_ADDRESS + (5 * digit);
}

// FX33 - Stores the Binary-coded decimal representation of VX
// at the addresses I, I plus 1, and I plus 2
void Chip8::OP_FX33()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t value = registers[Vx];

	 memory[index] = Vx % 1000 / 100;
    memory[index + 1] = Vx % 100 / 10;
    memory[index + 2] = Vx % 10;
}

// FX55 - Stores V0 to VX in memory starting at address I            
void Chip8::OP_FX55()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	for (uint8_t i =0; i<= Vx;++i)
    {
        memory[index +i] = registers[i]; 
    }
}

// FX65 - Reads V0 to VX in memory starting at address I   
void Chip8::OP_FX65()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	for (uint8_t i =0; i<= Vx;++i)
    {
        registers[i]= memory[index +i]; 
    }
}
