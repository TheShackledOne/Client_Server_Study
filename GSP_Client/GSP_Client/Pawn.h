#pragma once

#include "Common.h"
#include "Protocol.h"

class Pawn {
public:
	Pawn(const std::string& imagePath, const sf::Vector2f& position, 
		std::string& playerName, sf::Font& font);

	static void GetPawnId(SOCKET server_s, char* buf, WSABUF wsabuf[],
		float& initX, float& initY, int& pawnId);

	void draw(sf::RenderWindow& window);
	void move(float dx, float dy);

private:
	sf::Texture texture_;
	sf::Sprite	sprite_;
	sf::Font&	font_;
	sf::Text	text_;
};