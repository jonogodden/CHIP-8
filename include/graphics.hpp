//display hpp
#pragma once

#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SFML/Graphics.hpp>

class Graphics {
public:
    Graphics(unsigned int width, unsigned int height, unsigned int scale);
    void draw(const bool* gfx);  // display is the Chip-8's graphics buffer
    void clear();
    bool isOpen() const;
    void handleEvents(); // To handle window events like close

private:
    sf::RenderWindow window;
    sf::RectangleShape pixel;
    unsigned int width, height, scale;
};

#endif
