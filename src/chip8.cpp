
#include "chip8.hpp" // function declarations
#include "graphics.hpp"
#include <iostream>
#include <windows.h>
#include <fstream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <bitset>

Chip8::Chip8() : I(0), PC(0x200), SP(0), delay_timer(0), sound_timer(0), opcode(0), romSize(0) {
    // Initialise general-purpose registers
    std::fill(V.begin(), V.end(), 0);

    // Initialise memory
    std::fill(memory.begin(), memory.end(), 0);

    // Initialise display
    std::fill(display.begin(), display.end(), 0);

    // Initialise gfx
    std::fill(std::begin(gfx), std::end(gfx), false);


    // Initialise keypad
    std::fill(keypad.begin(), keypad.end(), 0);

    // Load the font set into memory
    for (size_t i = 0; i < font.size(); ++i) {
        memory[0x050 + i] = font[i];
    }

   


    // Debug output
    //// std::cout << "CHIP-8 Initialized: Memory, Registers, and Timers set to 0." << std::endl;
}



// load rom into memory

void Chip8::LoadGame(const char* filename) {
    // Create ifstream object to read file in binary mode
    std::ifstream file(filename, std::ios::binary);

    // Check if file opened successfully
    if (!file) {
        std::cerr << "Failed to load game: " << filename << std::endl;
        return;
    }

    // Get the size of the file
    file.seekg(0, std::ios::end);         // Move to the end of the file
    size_t size = file.tellg();           // Get the file size
    file.seekg(0, std::ios::beg);         // Move back to the beginning of the file

    // Check if the ROM is too large (max 4KB for CHIP-8 memory)
    if (size > (4096 - 0x200)) {
        std::cerr << "ROM is too large to fit in memory!" << std::endl;
        return;
    }

    // Read the ROM into a buffer
    std::vector<char> buffer(size);      // Use vector instead of manual memory management
    file.read(buffer.data(), size);      // Read file into the buffer
    file.close();                        // Close the file

    // Copy ROM data into memory starting at 0x200
    std::memcpy(&memory[0x200], buffer.data(), size);

    // Print the ROM size for debugging
    //// std::cout << "Loaded ROM with size: " << size << " bytes." << std::endl;

    // Call PrintRomHex with the correct size for debugging
    //// PrintRomHex(memory, size);  // Use the actual size of the ROM

    romSize = size;
}


// Print the ROM contents for verification

void Chip8::PrintRomHex(const uint8_t* memory, size_t size) {
	std::cout << "ROM Contents (in hexadecimal):" << std::endl;
    for (size_t i = 0x200; i < (0x200 + size); ++i) {  // Start at address 0x200
		std::cout << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)(unsigned char)memory[i] << " ";
		if ((i + 1) % 16 == 0) {  // Print 16 bytes per line
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

// Fetch Op Code

void Chip8::FetchOpCode() {


    // Combine two consecutive bytes in memory to form a 16-bit opcode
    // PC initialised to 0x200 in CHIP-8 constructor which is where the rom is loaded
    opcode = (memory[PC] << 8) | memory[PC + 1];

    


    // Increment PC by 2 (CHIP-8 instructions are 2 bytes long)
    PC += 2;

    // Debug output
    std::cout << "Fetched Opcode: 0x" << std::hex << std::uppercase << opcode << std::endl;
  //  std::cout << "Fetched Opcode: " << std::bitset<16>(opcode) << std::endl;

}






bool Chip8::KeyPress(uint8_t key) {
    // Mapping Chip-8 keys to keyboard keys using GetKeyState
    switch (key) {
    case 0x1:
        if (GetKeyState('1') & 0x8000) return true;  // '1' key
        break;
    case 0x2:
        if (GetKeyState('2') & 0x8000) return true;  // '2' key
        break;
    case 0x3:
        if (GetKeyState('3') & 0x8000) return true;  // '3' key
        break;
    case 0x4:
        if (GetKeyState('4') & 0x8000) return true;  // '4' key
        break;
    case 0x5:
        if (GetKeyState('Q') & 0x8000) return true;  // 'Q' key
        break;
    case 0x6:
        if (GetKeyState('W') & 0x8000) return true;  // 'W' key
        break;
    case 0x7:
        if (GetKeyState('E') & 0x8000) return true;  // 'E' key
        break;
    case 0x8:
        if (GetKeyState('A') & 0x8000) return true;  // 'A' key
        break;
    case 0x9:
        if (GetKeyState('S') & 0x8000) return true;  // 'S' key
        break;
    case 0xA:
        if (GetKeyState('D') & 0x8000) return true;  // 'D' key
        break;
    case 0xB:
        if (GetKeyState('Z') & 0x8000) return true;  // 'Z' key
        break;
    case 0xC:
        if (GetKeyState('C') & 0x8000) return true;  // 'C' key
        break;
    case 0xD:
        if (GetKeyState('4') & 0x8000) return true;  // '4' key
        break;
    case 0xE:
        if (GetKeyState('R') & 0x8000) return true;  // 'R' key
        break;
    case 0xF:
        if (GetKeyState('F') & 0x8000) return true;  // 'F' key
        break;
    }
    return false;
}



void Chip8::DecodeOp() {

    const uint8_t F = (opcode & 0xf000) >> 12;  // FXYN - 4 bits each
    const uint8_t X = (opcode & 0x0f00) >> 8;
    const uint8_t Y = (opcode & 0x00f0) >> 4;
    const uint8_t N = (opcode & 0x000f);
    const uint8_t NN = (opcode & 0x00ff);       // FXNN - 8 bits
    const uint16_t NNN = (opcode & 0x0fff);     // FNNN - 12 bits
    const uint8_t height = (opcode & 0x000f);   // height in display opcode 'N'
    uint16_t sum = 0;
    uint8_t randNum;
    uint8_t pixel;
    uint8_t xPos;
    uint8_t yPos;
    uint8_t spriteByte;
    uint8_t spritePixel;
    bool* screenPixel;

    switch ((opcode & 0xF000) >> 12) {

    case 0x0:
        switch (opcode) {
        case 0x00E0: // CLEAR SCREEN
            std::cout << "00E0" << std::endl;

            for (int i = 0; i < 64 * 32; ++i) {
                gfx[i] = false;  // All pixels off
            }
            
            return;

        case 0x00EE: // RETURN FROM SUBROUTINE
            std::cout << "00EE" << std::endl;

            PC = stack.top();
            stack.pop();

            return;

        default: // CALL MACHINE CODE ROUTINE?????
            std::cout << "0NNN" << std::endl;


            return;
        }

    case 0x1:
        std::cout << "1NNN" << std::endl;

        PC = NNN;

        return;

    case 0x2:
        std::cout << "2NNN" << std::endl;

        

        stack.push(PC);

        PC = NNN;
        

        return;

    case 0x3:
        std::cout << "3XNN" << std::endl;

        if (V.at(X) == NN) PC += 2;

        return;

    case 0x4:
        std::cout << "4XNN" << std::endl;

        if (V.at(X) != NN) PC += 2;

        return;

    case 0x5:
        std::cout << "5XY0" << std::endl;

        if (V.at(X) == V.at(Y)) PC += 2;

        return;

    case 0x6:
        std::cout << "6XNN" << std::endl;

        V.at(X) = NN;

        return;

    case 0x7:
        std::cout << "7XNN" << std::endl;

        V.at(X) += NN;

        return;

    case 0x8:
        switch (opcode & 0x000F) {
        case 0x0:
            std::cout << "8XY0" << std::endl;

            V.at(X) = V.at(Y);

            return;

        case 0x1:
            std::cout << "8XY1" << std::endl;

            V.at(X) = V.at(X) | V.at(Y);

            return;

        case 0x2:
            std::cout << "8XY2" << std::endl;

            V.at(X) = V.at(X) & V.at(Y);

            return;

        case 0x3:
            std::cout << "8XY3" << std::endl;

            V.at(X) = V.at(X) ^ V.at(Y);

            return;

        case 0x4:
            std::cout << "8XY4" << std::endl;

            sum = static_cast<unsigned>(V.at(X)) + static_cast<unsigned>(V.at(Y));
            if (sum > 0xff) {
                V.at(0xf) = 1;
            }
            else {
                V.at(0xf) = 0;
            }
            V.at(X) = sum & 0xff;

            return;

        case 0x5:
            std::cout << "8XY5" << std::endl;

            if (V.at(X) > V.at(Y)) {
                V.at(0xf) = 1;
            }
            else {
                V.at(0xf) = 0;
            }

            V.at(X) -= V.at(Y);
            return;

        case 0x6:
            std::cout << "8XY6" << std::endl;

            V.at(0xf) = (V.at(X) & 0x1);

            V.at(X) >>= 1;

            return;

        case 0x7:
            std::cout << "8XY7" << std::endl;

            if (V.at(Y) > V.at(X)) {
                V.at(0xf) = 1;
            }
            else {
                V.at(0xf) = 0;
            }

            V.at(X) = V.at(Y) - V.at(X);
         
            return;

        case 0xE:
            std::cout << "8XYE" << std::endl;

            V.at(0xf) = (V.at(X) & 0x80) >> 7;

            V.at(X) <<= 1;

            return;

        default:
            return;
        }

    case 0x9:
        std::cout << "9XY0" << std::endl;

        if (V.at(X) != V.at(Y)) {
            PC += 2;
        }

        return;

    case 0xA:
        std::cout << "ANNN" << std::endl;

        I = NNN;

        return;

    case 0xB:
        std::cout << "BNNN" << std::endl;

        PC = V.at(0) + NNN;

        return;

    case 0xC:
        std::cout << "CXNN" << std::endl;

        randNum = static_cast<uint8_t>(std::rand());
        V.at(X) = randNum & NN;

        return;

    case 0xD:
        std::cout << "DXYN" << std::endl;

        xPos = V.at(X) % 64;
        yPos = V.at(Y) % 32;

        V.at(0xf) = 0;

        for (unsigned int row = 0; row < height; ++row)
        {
            if (yPos + row >= 32) break;
            
            spriteByte = memory.at(I + row);

            for (unsigned int col = 0; col < 8; ++col)
            {
                if (xPos + col >= 64) break;

                spritePixel = spriteByte & (0x80u >> col);
                screenPixel = &gfx[(yPos + row) * 64 + (xPos + col)];

                // Sprite pixel is on
                if (spritePixel)
                {
                    // Screen pixel also on - collision
                    if (*screenPixel == true)
                    {
                        *screenPixel = false;
                        V.at(0xf) = 1;
                    }
                    else {
                        // Effectively XOR with the sprite pixel
                        *screenPixel ^= true;
                    }
                }
            }
        }

        return;

    case 0xE:
        switch (opcode & 0x00FF) {
        case 0x9E:
            std::cout << "EX9E" << std::endl;

            if (KeyPress(V.at(X))) {
                PC += 2;
           }

            return;

        case 0xA1:
            std::cout << "EXA1" << std::endl;


            if (!KeyPress(V.at(X))) {
                PC += 2;
            }

            return;

        default:
            return;
        }

    case 0xF:
        switch (opcode & 0x00FF) {
        case 0x07:
            std::cout << "FX07" << std::endl;

            V.at(X) = delay_timer;

            return;

        case 0x0A:
            std::cout << "FX0A" << std::endl;
            return;

        case 0x15:
            std::cout << "FX15" << std::endl;

            //delay_timer = V.at(X);

            return;

        case 0x18:
            std::cout << "FX18" << std::endl;

            sound_timer = V.at(X);

            return;

        case 0x1E:
            std::cout << "FX1E" << std::endl;

            //I =+ V.at(X);

            return;

        case 0x29:
            std::cout << "FX29" << std::endl;

            I = 0x050 + (V.at(X) * 5);

            return;

        case 0x33:
            std::cout << "FX33" << std::endl;
            return;

        case 0x55:
            std::cout << "FX55" << std::endl;
            return;

        case 0x65:
            std::cout << "FX65" << std::endl;
            return;

        default:
            return;

        }

    default:
        return;
    }


}

const bool* Chip8::getGraphics() const {
    return gfx;
}