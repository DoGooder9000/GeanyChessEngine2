#include "include/bitboard.h"

static const U64 IsAFile = 0x0101010101010101;
static const U64 IsHFile = 0x1010101010101010;
static const U64 IsBFile = IsAFile << 1;
static const U64 IsGFile = IsHFile >> 1;

static const U64 IsABFile = IsAFile | IsBFile;
static const U64 IsGHFile = IsGFile | IsHFile;

static const U64 IsRank8 = 0x00000000000000FF;
static const U64 IsRank7 = 0x000000000000FF00;
static const U64 IsRank2 = 0x00FF000000000000;
static const U64 IsRank1 = 0xFF00000000000000;

U64 PrecomputedPawnAttacks[2][64];
U64 PrecomputedPawnMoves[2][64];
U64 PrecomputedKnightMoves[64];
U64 PrecomputedKingMoves[64];