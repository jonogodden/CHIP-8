#include <fstream>
#include <iostream>
#include <filesystem>
#include "chip8.hpp"
#include "graphics.hpp"
#include <thread>


int main() {
    // Define the path to the ROM file (hardcoded)
    const char* romPath = "roms\\tetris.ch8";  // Correct path

    // Output the full path to check
    std::cout << "Trying to open ROM file: " << std::filesystem::absolute(romPath) << std::endl;

    // Check if the ROM file exists
    std::ifstream file(romPath);
    if (!file) {
        std::cerr << "Error: Could not open ROM file at " << romPath << std::endl;
        return 1;
    }
    else {
        std::cout << "ROM file loaded successfully: " << romPath << std::endl;
    }

    // Create an instance of the Chip8 class
    Chip8 chip8;


    // create graphics
    Graphics graphics(64, 32, 10);

    // Load the ROM file into the emulator's memory
    chip8.LoadGame(romPath);




    while (graphics.isOpen()) {

       
        graphics.handleEvents();

        chip8.FetchOpCode();
        chip8.DecodeOp();

        // Refresh display with the Chip-8 graphics buffer
        graphics.draw(chip8.getGraphics());

       

        std::this_thread::sleep_for(std::chrono::nanoseconds(1000));  // Slow it down

        
    }




    return 0;

 }