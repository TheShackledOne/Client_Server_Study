#include "Pawn.h"

Pawn::Pawn(sf::Color color, float radius, const sf::Vector2f& position)
{
	pawnShape_.setRadius(radius);
	pawnShape_.setFillColor(color);
	pawnShape_.setOrigin(radius, radius);
	pawnShape_.setPosition(position);
}

void Pawn::GetPawnId(SOCKET server_s, char* buf, WSABUF wsabuf[], float& initX, float& initY, int& pawnId)
{
	MovePacket* m_packet = reinterpret_cast<MovePacket*>(buf);

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
	//window.draw(sprite_);
	window.draw(pawnShape_);
}

void Pawn::move(float dx, float dy)
{
	//sprite_.move(dx, dy);
	pawnShape_.move(dx, dy);
}

void Pawn::setPosition(float x, float y)
{
	//sprite_.setPosition(x, y); // sf::Sprite의 setPosition 메서드를 사용
	pawnShape_.setPosition(x, y);
}
