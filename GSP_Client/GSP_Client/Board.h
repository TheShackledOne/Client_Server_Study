#pragma once

#include <SFML/Graphics.hpp>

#define SQUARE_SIZE 100

class Board {
public:
	Board();
	void draw(sf::RenderWindow& window);

private:
	sf::RectangleShape squares_[64];
};