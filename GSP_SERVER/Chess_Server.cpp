#include "Common.h"

int main()
{
	std::wcout.imbue(std::locale("korean"));
	WSAData wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata);

	SOCKET server_s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);
	SOCKADDR_IN addr_s;
	addr_s.sin_family = AF_INET;
	addr_s.sin_port = htons(SERVER_PORT);
	addr_s.sin_addr.s_addr = htonl(ADDR_ANY);
	inet_pton(AF_INET, SERVER_ADDR, &addr_s.sin_addr);

	if (bind(server_s, reinterpret_cast<sockaddr*>(&addr_s), sizeof(addr_s)) == SOCKET_ERROR) {
		int errCode = WSAGetLastError();
		std::cout << "bind 함수 실패, 에러 코드: " << errCode << std::endl;
	}
	listen(server_s, SOMAXCONN);

	char buf[BUFSIZE];
	WSABUF wsabuf[1];

	sockaddr addr_c;
	int addr_size = static_cast<int>(sizeof(addr_c));
	SOCKET client_s = WSAAccept(server_s, &addr_c, &addr_size, 0, 0);
	if (client_s != INVALID_SOCKET) {
		PawnManager::RegisterPawn(client_s, buf, wsabuf);
	}

	while (true) {
		// RECV
		wsabuf[0].buf = buf;
		wsabuf[0].len = sizeof(MovePacket);

		DWORD recv_size;
		DWORD recv_flag = 0;
		if (WSARecv(client_s, wsabuf, 1, &recv_size, &recv_flag, nullptr, nullptr) != 0) {
			int errCode = WSAGetLastError();
			std::cout << "WSARecv(Server) 실패, 에러 코드: " << errCode << std::endl;
		}
		if (recv_size == 0) break;

		// MoveProcess
		PawnManager::Move(buf);
		PawnManager::Update(buf);

		// Send
		wsabuf[0].buf = buf;
		wsabuf[0].len = sizeof(MovePacket);
		DWORD send_size;
		if (WSASend(client_s, wsabuf, 1, &send_size, 0, nullptr, nullptr) != 0) {
			int errCode = WSAGetLastError();
			std::cout << "WSASend(Server) 실패, 에러 코드: " << errCode << std::endl;
		}
	}
	closesocket(server_s);
	closesocket(client_s);
	WSACleanup();
}