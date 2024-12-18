#include "graphics.hpp"

Graphics::Graphics(unsigned int width, unsigned int height, unsigned int scale)
    : width(width), height(height), scale(scale), window(sf::VideoMode(width* scale, height* scale), "Chip-8 Emulator") {
    pixel.setSize(sf::Vector2f(scale, scale));
}

void Graphics::draw(const bool* gfx) {
    window.clear(sf::Color::Black);

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            if (gfx[y * width + x]) {
                pixel.setPosition(x * scale, y * scale);
                pixel.setFillColor(sf::Color::White);
                window.draw(pixel);
            }
        }
    }

    window.display();
}

void Graphics::clear() {
    window.clear(sf::Color::Black);
    window.display();
}

bool Graphics::isOpen() const {
    return window.isOpen();
}

void Graphics::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}
