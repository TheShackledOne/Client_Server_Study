#pragma once

#include "Common.h"
#include "Protocol.h"

class Pawn {
public:
	Pawn() { std::cout << "ERROR"; exit(-1); }
	Pawn(sf::Color color, float radius, const sf::Vector2f& position);

	//Pawn(const std::string& imagePath, const sf::Vector2f& position) {
	//	if (!texture_.loadFromFile(imagePath)) {
	//		std::cout << "이미지 로드 실패" << imagePath << std::endl;
	//	}
	//	sprite_.setTexture(texture_);
	//	sprite_.setPosition(position);
	//	/*text_.setFont(font);
	//	text_.setString(playerName);
	//	text_.setCharacterSize(36);
	//	text_.setFillColor(sf::Color::Cyan);
	//	sf::FloatRect textBounds = text_.getLocalBounds();
	//	text_.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
	//	text_.setPosition(sprite_.getPosition().x + 50, sprite_.getPosition().y + 50);*/
	//}

	static void GetPawnId(SOCKET server_s, char* buf, WSABUF wsabuf[],
		float& initX, float& initY, int& pawnId);

	void draw(sf::RenderWindow& window);
	void move(float dx, float dy);
	void setPosition(float x, float y); // setPosition 메서드 선언

private:
	sf::Texture texture_;
	sf::Sprite	sprite_;
	sf::CircleShape pawnShape_;
};