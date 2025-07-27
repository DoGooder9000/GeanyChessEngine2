#pragma once

#include "board.h"

#define U64 unsigned long long

#define GetBit(b, i) ((b) & (1ULL << (i))) // Usually returns a pretty large number if true, have to use a ? 1 : 0 to convert to 1 or 0
#define SetBit(b, i) ((b) |= (1ULL << (i)))
#define ClearBit(b, i) ((b) &= ~(1 << (i)))
#define PopBit(b, i) ((b) &= ~(1ULL << (i)))

extern U64 PrecomputedPawnAttacks[2][64];
extern U64 PrecomputedPawnMoves[2][64];
extern U64 PrecomputedKnightMoves[64];
extern U64 PrecomputedKingMoves[64];

U64 PawnAttackBitboard(int sq, int color);
U64 PawnMoveBitboard(int sq, int color);
U64 KnightMoveBitboard(int sq);
void PrecomputePawnTables(U64 attacks[2][64], U64 moves[2][64]);
void PrecomputeKnightTables(U64 moves[64]);
void PrecomputeKingTables(U64 moves[64]);