#include "Pawn.h"
#include "Board.h"

Pawn::Pawn(const std::string& imagePath, const sf::Vector2f& position, std::string& playerName, sf::Font& font)
	: font_{ font }
{
	texture_.loadFromFile(imagePath);
	sprite_.setTexture(texture_);
	sprite_.setPosition(position);
	
	text_.setFont(font);
	text_.setString(playerName);
	text_.setCharacterSize(36);
	text_.setFillColor(sf::Color::Cyan);
	sf::FloatRect textBounds = text_.getLocalBounds();
	text_.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
	text_.setPosition(sprite_.getPosition().x + 50, sprite_.getPosition().y + 50);
}

void Pawn::draw(sf::RenderWindow& window)
{
	window.draw(sprite_);
	window.draw(text_);
}

void Pawn::move(sf::Keyboard::Key direction)
{
	float deltaX{ 0.f };
	float deltaY{ 0.f };

	switch (direction) {
	case sf::Keyboard::Left:
		deltaX = -SQUARE_SIZE;
		break;
	case sf::Keyboard::Right:
		deltaX = SQUARE_SIZE;
		break;
	case sf::Keyboard::Up:
		deltaY = -SQUARE_SIZE;
		break;
	case sf::Keyboard::Down:
		deltaY = SQUARE_SIZE;
		break;
	default:
		return;
	}

	sf::Vector2f newPostion = sprite_.getPosition() + sf::Vector2f(deltaX, deltaY);
	if (newPostion.x >= 0 && newPostion.x <= SQUARE_SIZE * 7 &&
		newPostion.y >= 0 && newPostion.y <= SQUARE_SIZE * 7) {
		sprite_.move(deltaX, deltaY);
		text_.move(deltaX, deltaY);
	}
}
