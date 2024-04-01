#pragma once
#include "Common.h"
#include <string>

typedef enum OperationType : unsigned char {
    OP_GAMESTATE = 1,
    OP_MOVE = 2,
    OP_BROADCAST = 3,
    OP_LOGOUT = 4
}OP;

struct MovePacket {
    unsigned char size;
    unsigned char Id;
    unsigned char type;
    float x;
    float y;
    float dx;
    float dy;
    sf::Keyboard::Key direction;
};

struct UpdatePacket {
    int type;
    MovePacket m_packet[10];
    int packet_count;
};