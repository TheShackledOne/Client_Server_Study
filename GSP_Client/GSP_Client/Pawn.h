#pragma once

#include <SFML/Graphics.hpp>

class Pawn {
public:
	Pawn(const std::string& imagePath, const sf::Vector2f& position, std::string& playerName, sf::Font& font);
	void draw(sf::RenderWindow& window);
	void move(const sf::Keyboard::Key direction);

private:
	sf::Texture texture_;
	sf::Sprite	sprite_;
	sf::Font&	font_;
	sf::Text	text_;
};