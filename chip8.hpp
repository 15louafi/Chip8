#include <cstdint>
#include <random>

const unsigned int MEMORY_SIZE = 4096;
const unsigned int REGISTER_COUNT = 16;
const unsigned int STACK_LEVELS = 16;
const unsigned int KEY_COUNT = 16;
const unsigned int DISPLAY_HEIGHT = 32;
const unsigned int DISPLAY_WIDTH = 64;

class Chip8
{
public:
	Chip8();
	void init();
	void LoadROM(char const* filename);
	void EmulateCycle();
	uint8_t keyPad[16]{};
    uint32_t display[DISPLAY_HEIGHT * DISPLAY_WIDTH]{};

private:
    void ExecuteOpcode();
	// Do nothing
	void OP_NULL();

	// CLS
	void OP_00E0();

	// RET
	void OP_00EE();

	// JP address
	void OP_1NNN();

	// CALL address
	void OP_2NNN();

	// SE Vx, byte
	void OP_3XKK();

	// SNE Vx, byte
	void OP_4XKK();

	// SE Vx, Vy
	void OP_5XY0();

	// LD Vx, byte
	void OP_6XKK();

	// ADD Vx, byte
	void OP_7XKK();

	// LD Vx, Vy
	void OP_8XY0();

	// OR Vx, Vy
	void OP_8XY1();

	// AND Vx, Vy
	void OP_8XY2();

	// XOR Vx, Vy
	void OP_8XY3();

	// ADD Vx, Vy
	void OP_8XY4();

	// SUB Vx, Vy
	void OP_8XY5();

	// SHR Vx
	void OP_8XY6();

	// SUBN Vx, Vy
	void OP_8XY7();

	// SHL Vx
	void OP_8XYE();

	// SNE Vx, Vy
	void OP_9XY0();

	// LD I, address
	void OP_ANNN();

	// JP V0, address
	void OP_BNNN();

	// RND Vx, byte
	void OP_CXKK();

	// DRW Vx, Vy, height
	void OP_DXYN();

	// SKP Vx
	void OP_EX9E();

	// SKNP Vx
	void OP_EXA1();

	// LD Vx, DT
	void OP_FX07();

	// LD Vx, K
	void OP_FX0A();

	// LD DT, Vx
	void OP_FX15();

	// LD ST, Vx
	void OP_FX18();

	// ADD I, Vx
	void OP_FX1E();

	// LD F, Vx
	void OP_FX29();

	// LD B, Vx
	void OP_FX33();

	// LD [I], Vx
	void OP_FX55();

	// LD Vx, [I]
	void OP_FX65();

    uint8_t registers[REGISTER_COUNT]{};
    uint8_t memory[MEMORY_SIZE]{};
    uint16_t index{};
    uint16_t programCounter{};
    uint16_t stack[STACK_LEVELS]{};
    uint8_t stackPointer{};
    uint8_t delayTimer{};
    uint8_t soundTimer{};
    uint16_t opcode{};
};
