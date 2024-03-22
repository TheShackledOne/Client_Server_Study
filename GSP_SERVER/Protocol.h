#pragma once
#include "Common.h"
#include "SFML/Graphics.hpp"

enum OperationCode {
    OP_INIT = 1,
    OP_MOVE = 2

};

struct PacketHeader {
    unsigned int length;
    OperationCode opCode;
};

struct MovePacket {
    PacketHeader header;
    unsigned char Id;
    float x;
    float y;
    float dx;
    float dy;
    sf::Keyboard::Key direction;
};