#include <chrono>
#include <iostream>
#include <thread>
#include "chip8.hpp"
#include "display.hpp"
#include <SDL2/SDL.h>

const unsigned int WINDOW_WIDTH = 1024;
const unsigned int WINDOW_HEIGHT = 512;

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
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <ROM>\n";
		std::exit(EXIT_FAILURE);
	}

	char const* romFilename = argv[1];

	Display display("CHIP-8 Emulator", WINDOW_WIDTH, WINDOW_HEIGHT, DISPLAY_WIDTH, DISPLAY_HEIGHT);

	Chip8 chip8 = Chip8();
	chip8.LoadROM(romFilename);

	int videoPitch = sizeof(chip8.display[0]) * DISPLAY_WIDTH;

	bool quitKeyPressed = false;

	while (!quitKeyPressed)
	{
		quitKeyPressed = ProcessKeys(chip8.keyPad);

		chip8.EmulateCycle();

		display.Update(chip8.display, videoPitch);

		std::this_thread::sleep_for(std::chrono::microseconds(1200));
	}

	return 0;
}
