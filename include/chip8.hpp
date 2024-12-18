#pragma once

#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <array>
#include <cstdint>
#include <vector>
#include "defs.hpp"
#include "font.hpp"
#include <stack>

class Chip8 {
public:

    Chip8();

    size_t romSize;

    // chip8 methods

    void LoadGame(const char* filename);
 
    void PrintRomHex(const uint8_t* memory, size_t size);

    void FetchOpCode();

    void DecodeOp();

    const bool* getGraphics() const;

    bool KeyPress(uint8_t key);



private:


    //registers

    std::array<uint8_t, 4096> memory;       // 4kb of memory
                                            // 0x000 to 0x1FF Reserved for Interpreter
                                            // 0x050 to 0x0A0 For font set
                                            // 0x200 to 0xFFF for program memory
    std::array<uint8_t, 16> V;              // 16 registers V0 - VF
    uint16_t I;                             // Index register
    uint16_t PC;                            // program counter
    uint8_t SP;                              // stack pointer

    

    std::stack<uint16_t> stack;

    uint8_t delay_timer;        // decrements at 60Hz
    uint8_t sound_timer;        // Produces sound while >0

    std::array<uint8_t, 64 * 32> display;   // 64*32 pixel display

    bool gfx[64 * 32];

    std::array<uint8_t, 16> keypad;         // current keypad state

    uint16_t opcode;            // current opcode

 
  
};

#endif
