#pragma once
#include "Common.h"
#include "Protocol.h"

class SESSION;

extern bool b_shutdown;
extern std::unordered_map<LPWSAOVERLAPPED, int> g_session_map;
extern std::unordered_map<int, SESSION> g_players;

extern void CALLBACK send_callback(DWORD err, DWORD send_size, LPWSAOVERLAPPED pover, DWORD recv_flag);
extern void CALLBACK recv_callback(DWORD err, DWORD recv_size, LPWSAOVERLAPPED pover, DWORD recv_flag);

class EXP_OVER {
public:
	WSAOVERLAPPED over;
	WSABUF wsabuf[1];
	char buf[BUFSIZE];

	EXP_OVER(int send_id, char* message, int message_size) {
		ZeroMemory(&over, sizeof(over));
		wsabuf[0].buf = buf;
		wsabuf[0].len = message_size;

		memcpy(buf, message, message_size);
	}
};

class SESSION {
public:
	SESSION() {
		std::cout << "ERROR";
		exit(-1);
	};
	SESSION(SOCKET s, int id) : client_s(s), my_id(id) {
		g_session_map[&over] = my_id;
		wsabuf[0].buf = buf;
		wsabuf[0].len = BUFSIZE;
		ZeroMemory(&over, sizeof(over));
		initializePos(buf);
	}
	~SESSION() { 
		OP op = OP_LOGOUT;
		broadcast(op);
		std::cout << "**Client[" << this->my_id << "]" << "has DISCONNECTED !" << std::endl;
		g_players.erase(this->my_id);
		closesocket(client_s); 
	}
	//=============================================================================================//
	void do_recv() {
		DWORD recv_flag = 0;
		ZeroMemory(&over, sizeof(over));

		int res = WSARecv(client_s, wsabuf, 1, nullptr, &recv_flag, &over, recv_callback);
		if (0 != res) {
			int err_no = WSAGetLastError();
			if (WSA_IO_PENDING != err_no) error_display("WSA_Recv_Server", WSAGetLastError());
		}
	}
	void do_send(int send_id, char* message, DWORD recv_size) {
		auto b = new EXP_OVER(send_id, message, recv_size);
		//==SEND와 RECV가 같은 구조체를 사용하지 않도록 EXP_OVER의 구조체를 사용한다==//
		int res = WSASend(client_s, b->wsabuf, 1, nullptr, 0, &b->over, send_callback);
		if (0 != res) {
			int err_no = WSAGetLastError();
			if (WSA_IO_PENDING != err_no) error_display("WSA_Send_Server", WSAGetLastError());
		}
	}
	void broadcast(OP op) {
		MovePacket packet;
		if (op == OP_LOGOUT) {
			for (auto& pair : g_players) {
				if (pair.first != this->my_id) {
					packet.Id = this->my_id;
					packet.type = op;
					packet.x = this->x;
					packet.y = this->y;
					memcpy(pair.second.buf, &packet, sizeof(MovePacket));
					pair.second.do_send(pair.first, pair.second.buf, sizeof(MovePacket));
				}
			}
		}
		else {
			for (auto& pair : g_players) {
				packet.Id = pair.first;
				packet.type = op;
				packet.x = pair.second.x;
				packet.y = pair.second.y;
				memcpy(this->buf, &packet, sizeof(MovePacket));
				this->do_send(this->my_id, this->buf, sizeof(MovePacket));
			}
			for (auto& pair : g_players) {
				if (pair.first != this->my_id) {
					packet.Id = this->my_id;
					packet.type = op;
					packet.x = this->x;
					packet.y = this->y;
					memcpy(pair.second.buf, &packet, sizeof(MovePacket));
					pair.second.do_send(pair.first, pair.second.buf, sizeof(MovePacket));
				}
			}
		}
		
	}
	void initializePos(char buf[]) {
		MovePacket* packet = reinterpret_cast<MovePacket*>(buf);
		packet->x = 50;
		packet->y = 50;
		packet->Id = my_id;

		this->x = 50;
		this->y = 50;

		memcpy(buf, packet, sizeof(MovePacket));

		do_send(my_id, buf, sizeof(MovePacket));
	
		OP op = OP_GAMESTATE;
		broadcast(op);

	}
	bool isMovePacket() const {
		const MovePacket* packet = reinterpret_cast<const MovePacket*>(buf);
		return packet->type == OP_MOVE;
	}
	bool isGameState() const {
		const MovePacket* packet = reinterpret_cast<const MovePacket*>(buf);
		return packet->type == OP_GAMESTATE;
	}
	void Move(int my_id);
	//=============================================================================================//
private:
	char buf[BUFSIZE];
	WSABUF wsabuf[1];
	SOCKET client_s;
	WSAOVERLAPPED over;
	
	float x;
	float y;
	int my_id;
	//char name[MAX_LENGTH];
};
