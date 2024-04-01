#include "PawnManager.h"
#include "Protocol.h"

bool b_shutdown = false;
std::unordered_map<LPWSAOVERLAPPED, int> g_session_map;
std::unordered_map<int, SESSION> g_players;

void CALLBACK send_callback(DWORD err, DWORD send_size, LPWSAOVERLAPPED pover, DWORD recv_flag)
{
	int my_id = g_session_map[pover];
	if (0 != err) {
		error_display("WSASend", WSAGetLastError());
	}
	auto b = reinterpret_cast<EXP_OVER*>(pover);
	delete b; //==send_callback은 memory leak만 관리==//
}

void CALLBACK recv_callback(DWORD err, DWORD recv_size, LPWSAOVERLAPPED pover, DWORD recv_flag)
{
	int my_id = g_session_map[pover];
	if (0 == recv_size) {
		g_players.erase(my_id);	// Overlapped 구조체를 index로
		return;
	}
	if (g_players[my_id].isMovePacket()) {
		g_players[my_id].Move(my_id);
		OP op = OP_MOVE;
		g_players[my_id].broadcast(op);
	}
	g_players[my_id].do_recv();
}

void SESSION::Move(int my_id)
{
	MovePacket* m_packet = reinterpret_cast<MovePacket*>(buf);

	std::string direc;
	float dx = 0;
	float dy = 0;

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

	if (g_players[my_id].x + dx < 0 || g_players[my_id].x + dx > SQUARE_SIZE * 7 + 50
		|| g_players[my_id].y + dy < 0 || g_players[my_id].y + dy > SQUARE_SIZE * 7 + 50) {
		dx = 0;
		dy = 0;
		std::cout << "보드 밖으로 벗어날 수 없습니다." << std::endl;
	}

	float newX = g_players[my_id].x + dx;
	float newY = g_players[my_id].y + dy;
	m_packet->x = newX;
	m_packet->y = newY;
	m_packet->dx = dx;
	m_packet->dy = dy;

	std::cout << "Client[" << my_id << "] " << direc << " (" << newX - 50
		<< ", " << newY - 50 << ")" << std::endl;

	//=========================//
	g_players[my_id].x = newX;
	g_players[my_id].y = newY;
	//=========================//

	memcpy(buf, m_packet, sizeof(MovePacket));
	do_send(my_id, buf, sizeof(MovePacket));
}