#include "Common.h"
#include "PawnManager.h"
#include "Protocol.h"

int main()
{
	std::wcout.imbue(std::locale("korean"));
	WSAData wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata);

	SOCKET server_s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN addr_s;
	addr_s.sin_family = AF_INET;
	addr_s.sin_port = htons(SERVER_PORT);
	addr_s.sin_addr.s_addr = htonl(ADDR_ANY);
	inet_pton(AF_INET, SERVER_ADDR, &addr_s.sin_addr);

	bind(server_s, reinterpret_cast<sockaddr*>(&addr_s), sizeof(addr_s));
	listen(server_s, SOMAXCONN);

	sockaddr addr_c;
	int addr_size = static_cast<int>(sizeof(addr_c));
	int id = 1;

	while (false == b_shutdown) {
		//==================================================================//
		SOCKET client_s = WSAAccept(server_s, &addr_c, &addr_size, 0, 0);
		if (client_s != INVALID_SOCKET) std::cout << "**Client[" << id << "] has CONNECTED !" << std::endl;
		g_players.try_emplace(id, client_s, id);
		g_players[id++].do_recv();
		//==================================================================//
	}
	g_players.clear();
	closesocket(server_s);
	WSACleanup();
}