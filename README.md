# Chip 8 Emulator
CHIP-8 emulator written in C++ with SDL.
CHIP-8 is an interpreted programming language, developed by Joseph Weisbecker. It was initially used on the COSMAC VIP and Telmac 1800 8-bit microcomputers in the mid-1970s. CHIP-8 programs are run on a CHIP-8 virtual machine. It was made to allow video games to be more easily programmed for these computers. 
Code follows specs defined in [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#Ex9E) and [CHIP-8 Wikipedia page](https://en.wikipedia.org/wiki/CHIP-8).

## Screenshots

![Space Invaders](https://raw.githubusercontent.com/15louafi/Chip8/main/screenshots/SpaceInvaders.png)


![Tetris](https://github.com/15louafi/Chip8/blob/main/screenshots/Tetris.png)

## Prerequisites
* C++ compiler
* SDL2
* Cmake

## How to use

Cmake and SDL2 are required for this project.

```
$ sudo apt-get install cmake libsdl2-dev
```

Then, to compile the code, you need to run

```
$ cmake .
$ make
```
## Running

Only one argument is required: the path to the ROM file you want to run.

```
./Chip8 PATH_TO_ROM
```

## Controls
The CHIP-8 uses a 16-key keypad and is mapped to the following keys:

     1 2 3 4
     q w e r
     a s d f
     z x c v
     
## Work in progress
* Implement sound
* Add unit tests
* Add more commands for debugging (trigger verbose output)
* Fix more bugs
* Allow for window resizing

## Ressources
* [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#Ex9E)
* [CHIP-8 Wikipedia page](https://en.wikipedia.org/wiki/CHIP-8).
