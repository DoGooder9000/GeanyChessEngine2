#pragma once

#include "board.h"
#include "bitboard.h"

void GenerateMoves(Board* b, MoveList* moves, int quiet);
void GeneratePawnMoves(Board* b, MoveList* moves, int quiet);