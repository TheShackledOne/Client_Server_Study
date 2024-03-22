#pragma once

#include "Common.h"

class Board {
public:
	Board();
	void draw(sf::RenderWindow& window);

private:
	sf::RectangleShape squares_[64];
};