#include "Common.h"

void PawnManager::RegisterPawn(SOCKET client_s, char* buf, WSABUF wsabuf[])
{
	//MovePacket m_packet;

	//memcpy(buf, &m_packet, sizeof(m_packet));
	wsabuf[0].buf = buf;
	wsabuf[0].len = BUFSIZE;
	DWORD recv_size;
	DWORD recv_flag = 0;
	WSARecv(client_s, wsabuf, 1, &recv_size, &recv_flag, nullptr, nullptr);

	MovePacket* recv_packet = reinterpret_cast<MovePacket*>(buf);
	if (recv_packet->header.opCode == OP_INIT) {
		PawnManager& instance = GetInstance();
		recv_packet->x = 100;
		recv_packet->y = 100;
		recv_packet->Id = instance._pawnId++;
		instance.pawns[recv_packet->Id] = { recv_packet->x , recv_packet->y };

		memcpy(buf, recv_packet, sizeof(MovePacket));
		wsabuf[0].len = sizeof(MovePacket);
		WSASend(client_s, wsabuf, 1, &recv_size, 0, nullptr, nullptr);
	}
}

void PawnManager::Move(char* buf)
{
	MovePacket* m_packet = reinterpret_cast<MovePacket*>(buf);

	if (m_packet->header.opCode != OP_MOVE) {
		std::cout << "opCode is not OP_MOVE" << std::endl;
		return;
	}
	std::string direc;
	float dx = 0;
	float dy = 0;

	PawnManager& instance = GetInstance();
	auto it = instance.pawns.find(m_packet->Id);
	if (it != instance.pawns.end())
	{
		m_packet->x = instance.pawns[m_packet->Id].x;
		m_packet->y = instance.pawns[m_packet->Id].y;

		switch (m_packet->direction) {
		case sf::Keyboard::Left:
			dx = -SQUARE_SIZE;
			direc = "Move Left";
			break;
		case sf::Keyboard::Right:
			dx = SQUARE_SIZE;
			direc = "Move Right";
			break;
		case sf::Keyboard::Up:
			dy = -SQUARE_SIZE;
			direc = "Move Up";
			break;
		case sf::Keyboard::Down:
			dy = SQUARE_SIZE;
			direc = "Move Down";
			break;
		default:
			direc = "Wrong Input";
			return;
		}
	}

	if (m_packet->x + dx < 0 || m_packet->x + dx > SQUARE_SIZE * 7
		|| m_packet->y + dy < 0 || m_packet->y + dy > SQUARE_SIZE * 7) {
		dx = 0;
		dy = 0;
		std::cout << "보드 밖으로 벗어날 수 없습니다." << std::endl;
	}

	m_packet->dx = dx;
	m_packet->dy = dy;

	float newX = m_packet->x + m_packet->dx;
	float newY = m_packet->y + m_packet->dy;

	std::cout << direc << " (" << newX
		<< ", " << newY << ")" << std::endl;
}

void PawnManager::Update(char* buf)
{
	MovePacket* m_packet = reinterpret_cast<MovePacket*>(buf);

	PawnManager& instance = GetInstance();
	auto it = instance.pawns.find(m_packet->Id);
	if (it != instance.pawns.end()) {
		it->second.x += m_packet->dx;
		it->second.y += m_packet->dy;
	}
	else {
		std::cout << "Pawn [" << m_packet->Id
			<< "] 이 없습니다." << std::endl;
		exit(-1);
	}
}