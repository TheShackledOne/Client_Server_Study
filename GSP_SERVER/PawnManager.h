#pragma once
#include "Common.h"
#include "SFML/Graphics.hpp"

typedef struct pawn {
	float x, y;
}Pawn;

class PawnManager {
public:
	static PawnManager& GetInstance() { static PawnManager instance; return instance; }

	static void RegisterPawn(SOCKET client_s, char* buf, WSABUF wsabuf[]);
	static void Move(char* buf);
	static void Update(char* buf);

private:
	PawnManager() {};

	std::unordered_map<int, Pawn> pawns;
	unsigned int _pawnId = 1;
};