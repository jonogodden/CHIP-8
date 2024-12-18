cmake_minimum_required(VERSION 3.22.1)

project(Chip8Emulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the 'include' directory for header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add SFML library path
set(SFML_DIR "C:/SFML/lib/cmake/SFML") # Change to your SFML path
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Set up source files
set(SOURCES
    src/main.cpp
    src/chip8.cpp
    src/display.cpp
)

# Set up header files (for Solution Explorer)
set(HEADERS
    include/chip8.hpp
    include/defs.hpp
    include/font.hpp
    include/display.hpp
)

# Group header files under a 'Header Files' folder in Solution Explorer
source_group("Header Files" FILES ${HEADERS})

# Add the executable (no need to list headers here)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)