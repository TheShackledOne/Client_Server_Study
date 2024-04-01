#pragma once
#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <unordered_map>
#pragma comment(lib, "WS2_32.LIB")
#include "SFML/Graphics.hpp"

constexpr char SERVER_ADDR[] = "127.0.0.1";
constexpr short SERVER_PORT = 4000;
constexpr int BUFSIZE = 256;
constexpr int MAX_LENGTH = 20;

#define SQUARE_SIZE 100

void error_display(const char* msg, int err_no);