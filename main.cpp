#include <chrono>
#include <iostream>
#include "chip8.hpp"
#include "display.hpp"
#include <SDL2/SDL.h>

const unsigned int WINDOW_HEIGHT = 1024;
const unsigned int WINDOW_WIDTH = 512;

bool ProcessKeys(uint8_t* keys)
{
	bool quitKeyPressed = false;

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				quitKeyPressed = true;
			} 
			break;

			case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
					{
						quitKeyPressed = true;
					} 
					break;

					case SDLK_x:
					{
						keys[0] = 1;
					} 
					break;

					case SDLK_1:
					{
						keys[1] = 1;
					} 
					break;

					case SDLK_2:
					{
						keys[2] = 1;
					} 
					break;

					case SDLK_3:
					{
						keys[3] = 1;
					} 
					break;

					case SDLK_q:
					{
						keys[4] = 1;
					} 
					break;

					case SDLK_w:
					{
						keys[5] = 1;
					} 
					break;

					case SDLK_e:
					{
						keys[6] = 1;
					} 
					break;

					case SDLK_a:
					{
						keys[7] = 1;
					} 
					break;

					case SDLK_s:
					{
						keys[8] = 1;
					} 
					break;

					case SDLK_d:
					{
						keys[9] = 1;
					} 
					break;

					case SDLK_z:
					{
						keys[0xA] = 1;
					} 
					break;

					case SDLK_c:
					{
						keys[0xB] = 1;
					} 
					break;

					case SDLK_4:
					{
						keys[0xC] = 1;
					} 
					break;

					case SDLK_r:
					{
						keys[0xD] = 1;
					} 
					break;

					case SDLK_f:
					{
						keys[0xE] = 1;
					} 
					break;

					case SDLK_v:
					{
						keys[0xF] = 1;
					} 
					break;
				}
			} 
			break;

			case SDL_KEYUP:
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_x:
					{
						keys[0] = 0;
					} 
					break;

					case SDLK_1:
					{
						keys[1] = 0;
					} 
					break;

					case SDLK_2:
					{
						keys[2] = 0;
					} 
					break;

					case SDLK_3:
					{
						keys[3] = 0;
					} 
					break;

					case SDLK_q:
					{
						keys[4] = 0;
					} 
					break;

					case SDLK_w:
					{
						keys[5] = 0;
					} 
					break;

					case SDLK_e:
					{
						keys[6] = 0;
					} 
					break;

					case SDLK_a:
					{
						keys[7] = 0;
					} 
					break;

					case SDLK_s:
					{
						keys[8] = 0;
					} 
					break;

					case SDLK_d:
					{
						keys[9] = 0;
					} 
					break;

					case SDLK_z:
					{
						keys[0xA] = 0;
					} 
					break;

					case SDLK_c:
					{
						keys[0xB] = 0;
					} 
					break;

					case SDLK_4:
					{
						keys[0xC] = 0;
					} 
					break;

					case SDLK_r:
					{
						keys[0xD] = 0;
					} 
					break;

					case SDLK_f:
					{
						keys[0xE] = 0;
					} 
					break;

					case SDLK_v:
					{
						keys[0xF] = 0;
					} 
					break;
				}
			} break;
		}
	}

	return quitKeyPressed;
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <Delay> <ROM>\n";
		std::exit(EXIT_FAILURE);
	}

	int cycleDelay = std::stoi(argv[1]);
	char const* romFilename = argv[2];

	Display display("CHIP-8 Emulator", WINDOW_HEIGHT, WINDOW_WIDTH, DISPLAY_WIDTH, DISPLAY_HEIGHT);

	Chip8 chip8 = Chip8();
	chip8.LoadROM(romFilename);

	int videoPitch = sizeof(chip8.display[0]) * DISPLAY_WIDTH;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quitKeyPressed = false;

	while (!quitKeyPressed)
	{
		quitKeyPressed = ProcessKeys(chip8.keyPad);

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		if (dt > cycleDelay)
		{
			lastCycleTime = currentTime;

			chip8.EmulateCycle();

			display.Update(chip8.display, videoPitch);
		}
	}

	return 0;
}
