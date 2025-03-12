# CHIP-8 Emulator

## Overview

This is a CHIP-8 emulator written in C++. It emulates the CHIP-8 virtual machine, allowing you to run classic CHIP-8 programs and games.

## What is CHIP-8?

CHIP-8 is a simple interpreted programming language developed in the 1970s, primarily used for running small games on early microcomputers and later on embedded systems. Despite its age, CHIP-8 remains a popular project for learning about emulation and low-level programming.

### Key Features of CHIP-8:

- 4K of memory (including ROM and RAM)
- 16 8-bit general-purpose registers (V0-VF)
- A stack for subroutine calls
- A 16-bit program counter
- A simple graphics system (64x32 monochrome display)
- 35 opcodes for basic operations
- A built-in timer and sound system
- A hexadecimal keypad input (0-9, A-F)

## Techniques Used in the Emulator

Building a CHIP-8 emulator requires an understanding of:

### **1. CPU Emulation**

- Fetch-Decode-Execute cycle: The emulator reads and processes CHIP-8 opcodes sequentially.
- Implementing CHIP-8’s 35 opcodes for arithmetic, memory operations, drawing, and control flow.

### **2. Memory Management**

- CHIP-8 programs are loaded at memory address `0x200`.
- Using an array to simulate the 4K memory space.

### **3. Graphics Rendering**

- The CHIP-8 screen is a 64x32 monochrome display.
- The emulator handles drawing using a framebuffer and XOR-based pixel operations.
- Display updates are done using a graphics library like SDL.

### **4. Input Handling**

- CHIP-8 uses a hexadecimal keypad (0-9, A-F).
- Mapping modern keyboard inputs to CHIP-8 keys.

### **5. Timers and Sound**

- Implementing delay and sound timers that decrement at 60Hz.
- Sound generation when the sound timer is active.

## How to Build and Run

### **Requirements:**

- C++ compiler (GCC, Clang, MSVC, etc.)
- SFML (if using for graphics and input handling)

### **Build Instructions:**

```sh
mkdir build && cd build
cmake ..
make
./chip8emulator path/to/rom.ch8
```

## Example ROMs

You can find CHIP-8 ROMs online for testing, such as:

- [Public domain CHIP-8 games](https://johnearnest.github.io/chip8Archive/)

## TODO & Future Improvements

- Implement Super CHIP-8 (SCHIP) support
- Add save states
- Improve UI with debugging tools

## License

This project is open-source and licensed under the MIT License.

