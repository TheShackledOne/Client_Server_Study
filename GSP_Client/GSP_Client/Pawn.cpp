#include "Pawn.h"

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

void Pawn::GetPawnId(SOCKET server_s, char* buf, WSABUF wsabuf[], float& initX, float& initY, int& pawnId)
{
	MovePacket* m_packet = reinterpret_cast<MovePacket*>(buf);
	m_packet->header.opCode = OP_INIT;

	wsabuf[0].buf = buf;
	wsabuf[0].len = sizeof(MovePacket);
	DWORD send_size;
	WSASend(server_s, wsabuf, 1, &send_size, 0, nullptr, nullptr);

	wsabuf[0].len = BUFSIZE;
	DWORD recv_size;
	DWORD recv_flag = 0;
	WSARecv(server_s, wsabuf, 1, &recv_size, &recv_flag, nullptr, nullptr);

	MovePacket* recv_packet = reinterpret_cast<MovePacket*>(buf);

	initX = recv_packet->x;
	initY = recv_packet->y;
	pawnId = recv_packet->Id;
}

void Pawn::draw(sf::RenderWindow& window)
{
	window.draw(sprite_);
	window.draw(text_);
}

void Pawn::move(float dx, float dy)
{
	sprite_.move(dx, dy);
	text_.move(dx, dy);
}