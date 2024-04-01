#include "Common.h"
#include "Protocol.h"
#include "Board.h"
#include "Pawn.h"
#pragma comment (lib, "WS2_32.LIB")

void CALLBACK send_callback(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void CALLBACK recv_callback(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ProcessPawn(char* buffer);
void SendMovePacket(sf::Keyboard::Key direction);
//===================================//
std::unordered_map<int, Pawn> pawns;
bool b_shutdown = false;
int myId = 0;

char buf[BUFSIZE];
WSABUF wsabuf[1];
SOCKET server_s;
WSAOVERLAPPED over;
//===================================//
class EXP_OVER {
public:
    WSAOVERLAPPED overlapped;
    WSABUF wsabuf;
    char buffer[BUFSIZE];
    int operation;  // 0: Recv, 1: Send

    EXP_OVER() {
        ZeroMemory(&overlapped, sizeof(overlapped));
        wsabuf.buf = buffer;
        wsabuf.len = BUFSIZE;
        operation = -1;
    }
};
//===================================//
void do_recv() {
    auto recv_over = new EXP_OVER();
    recv_over->operation = 0; // Recv
    DWORD flags = 0;
    WSARecv(server_s, &recv_over->wsabuf, 1, nullptr, &flags, &recv_over->overlapped, recv_callback);
}
void do_send(const char* buffer, int length) {
    auto send_over = new EXP_OVER();
    send_over->operation = 1; // Send
    memcpy(send_over->buffer, buffer, sizeof(MovePacket));
    send_over->wsabuf.len = sizeof(MovePacket);
    WSASend(server_s, &send_over->wsabuf, 1, nullptr, 0, &send_over->overlapped, send_callback);
}
void CALLBACK recv_callback(DWORD err, DWORD recv_size, LPWSAOVERLAPPED overlapped, DWORD flags) {
    EXP_OVER* exp_over = reinterpret_cast<EXP_OVER*>(overlapped);
    if (exp_over->operation == 0) { // Recv
        if (err == 0 && recv_size > 0) {
            ProcessPawn(exp_over->buffer);
        }
        do_recv();
    }
    delete exp_over;
}
void CALLBACK send_callback(DWORD err, DWORD send_size, LPWSAOVERLAPPED overlapped, DWORD flags) {
    EXP_OVER* exp_over = reinterpret_cast<EXP_OVER*>(overlapped);
    delete exp_over;
}
void ProcessPawn(char* buffer)
{
    MovePacket* recv_packet = reinterpret_cast<MovePacket*>(buffer);

    if (recv_packet->type == OP_LOGOUT) {
        pawns.erase(recv_packet->Id);
        return;
    }
    auto it = pawns.find(recv_packet->Id);
    if (it != pawns.end()) {
        // 해당 ID의 폰이 이미 존재하면 위치 업데이트
        it->second.setPosition(recv_packet->x, recv_packet->y);
        std::cout << "Client ID: " << static_cast<int>(recv_packet->Id) << ", x: "
            << recv_packet->x - 50 << ", y: " << recv_packet->y - 50 << std::endl;
    }
    else {
        // 존재하지 않는 폰이라면 새로운 폰 객체를 생성하고 목록에 추가
        Pawn newPawn(sf::Color::Red, static_cast<float>(SQUARE_SIZE / 4), sf::Vector2f(recv_packet->x, recv_packet->y));
        pawns.insert({ recv_packet->Id, std::move(newPawn) });
        std::cout << "**New Client ID: " << static_cast<int>(recv_packet->Id) << std::endl;
    }
}
void SendMovePacket(sf::Keyboard::Key direction)
{
    MovePacket m_packet;
    m_packet.type = OP_MOVE;
    m_packet.direction = direction;
    m_packet.Id = myId;

    // 송신 데이터를 do_send 함수를 통해 비동기 송신
    do_send(reinterpret_cast<char*>(&m_packet), sizeof(MovePacket));
}
//================================================================================================================//
int main()
{
    std::wcout.imbue(std::locale("korean"));
    WSAData wsadata;
    WSAStartup(MAKEWORD(2, 0), &wsadata);

    std::unique_ptr<Board> board = std::make_unique<Board>();

    char IP_ADDR[17];
    std::cout << "서버의 IP 주소를 입력하세요 : ";
    std::cin.getline(IP_ADDR, sizeof(IP_ADDR));

    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess_Client");

    server_s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
    SOCKADDR_IN addr_s;
    addr_s.sin_family = AF_INET;
    addr_s.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, IP_ADDR, &addr_s.sin_addr);

    float initX = 0;
    float initY = 0;

    int res = connect(server_s, reinterpret_cast<sockaddr*>(&addr_s), sizeof(addr_s));
    if (res != 0) {
        error_display("WSASend", WSAGetLastError());
    }
    else Pawn::GetPawnId(server_s, buf, wsabuf, initX, initY, myId);

    Pawn pawn(sf::Color::Green, static_cast<float>(SQUARE_SIZE / 4), sf::Vector2f(initX, initY));
    pawns.insert({ myId, pawn });
    do_recv();
    //==================while==================//
    while (window.isOpen())
    {
        SleepEx(0, TRUE);

        sf::Event event;
        sf::Vector2f currentPosition;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) SendMovePacket(event.key.code);
        }
        window.clear();
        board->draw(window);
        for (auto& pair : pawns) {
            pair.second.draw(window);
        }
        window.display();
    }
    closesocket(server_s);
    WSACleanup();
}
//================================================================================================================//