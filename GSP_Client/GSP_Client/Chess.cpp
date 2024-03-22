#include "Common.h"
#include "Protocol.h"
#include "Board.h"
#include "Pawn.h"
#pragma comment (lib, "WS2_32.LIB")

int main()
{
    std::wcout.imbue(std::locale("korean"));
    WSAData wsadata;
    WSAStartup(MAKEWORD(2, 0), &wsadata);

    std::unique_ptr<Board> board = std::make_unique<Board>();

    char IP_ADDR[17];
    std::cout << "서버의 IP 주소를 입력하세요 : ";
    std::cin.getline(IP_ADDR, sizeof(IP_ADDR));

    SOCKET server_s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);
    SOCKADDR_IN addr_s;
    addr_s.sin_family = AF_INET;
    addr_s.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, IP_ADDR, &addr_s.sin_addr);

    char buf[BUFSIZE];
    WSABUF wsabuf[1];

    std::string playerName;
    std::cout << "Enter your name : ";
    std::cin >> playerName;

    sf::Font font;
    font.loadFromFile("./Assets/Font/D2Coding.ttc");

    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess_Client");

    int pawnId = 0;
    float initX = 0;
    float initY = 0;

    int res = connect(server_s, reinterpret_cast<sockaddr*>(&addr_s), sizeof(addr_s));
    if (res != 0) {
        int errCode = WSAGetLastError();
        std::cout << "connect 함수 실패, 에러 코드: " << errCode << std::endl;
    }
    else Pawn::GetPawnId(server_s, buf, wsabuf, initX, initY, pawnId);

    Pawn pawn("./Assets/Image/pawn.png", sf::Vector2f(initX, initY), playerName, font);
    //------------------------------------------------------------------------------------//
    //------------------------------------------------------------------------------------//
    while (window.isOpen())
    {
        sf::Event event;
        sf::Vector2f currentPosition;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                // Input
                MovePacket m_packet;
                m_packet.header.opCode = OP_MOVE;
                m_packet.direction = event.key.code;
                m_packet.Id = pawnId;
                memcpy(buf, &m_packet, sizeof(MovePacket));

                // SEND
                DWORD send_size;
                wsabuf[0].buf = buf;
                wsabuf[0].len = sizeof(MovePacket);
                if (WSASend(server_s, wsabuf, 1, &send_size, 0, nullptr, nullptr) != 0) {
                    int errCode = WSAGetLastError();
                    std::cout << "WSASend(Client) 실패, 에러 코드: " << errCode << std::endl;
                }

                // RECV
                wsabuf[0].len = sizeof(MovePacket);
                DWORD recv_size;
                DWORD recv_flag = 0;
                if (WSARecv(server_s, wsabuf, 1, &recv_size, &recv_flag, nullptr, nullptr) != 0) {
                    int errCode = WSAGetLastError();
                    std::cout << "WSARecv(Client) 실패, 에러 코드: " << errCode << std::endl;
                }
                MovePacket* recv_packet = reinterpret_cast<MovePacket*>(buf);
                // MoveProcess
                pawn.move(recv_packet->dx, recv_packet->dy);
            }
        }
        window.clear();
        board->draw(window);
        pawn.draw(window);

        window.display();
    }
    closesocket(server_s);
    WSACleanup();
}