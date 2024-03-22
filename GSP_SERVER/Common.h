#pragma once
#define NOMINMAX

#include <iostream>
#include <WS2tcpip.h>
#include <unordered_map>
#include "Protocol.h"
#include "PawnManager.h"
#pragma comment(lib, "WS2_32.LIB")
#include "SFML/Graphics.hpp"

constexpr char SERVER_ADDR[] = "127.0.0.1";
constexpr short SERVER_PORT = 4000;
constexpr int BUFSIZE = 256;

#define SQUARE_SIZE 100