#include "include/bitboard.h"
#include "include/board.h"

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

static const U64 IsRank12 = IsRank1 | IsRank2;
static const U64 IsRank78 = IsRank7 | IsRank8;

U64 PrecomputedPawnAttacks[2][64];
U64 PrecomputedPawnMoves[2][64];
U64 PrecomputedKnightMoves[64];
U64 PrecomputedKingMoves[64];


int CountBits(U64 b){
	int num = 0;

	while (b){
		num++;
		b &= b-1;
	}
	
	return num;
}

int GetLSBIndex(U64 b){
	if (b){
		return CountBits((b & -b) - 1);
	}

	// If it is all 0's
	return -1;
}

U64 PawnAttackBitboard(int sq, int color){
	U64 PawnPos = 1ULL << sq;
	U64 b = 0ULL;
		
	if (!color){				
		if (!(PawnPos & IsAFile) && (PawnPos >> 9)){
			b |= PawnPos >> 9;
		}
		if (!(PawnPos & IsHFile) && (PawnPos >> 7)){
			b |= PawnPos >> 7;
		}
	}
	else {				
		if (!(PawnPos & IsAFile) && (PawnPos << 7)){
			b |= PawnPos << 7;
		}
		if (!(PawnPos & IsHFile) && (PawnPos << 9)){
			b |= PawnPos << 9;
		}
	}
	
	return b;
}

U64 PawnMoveBitboard(int sq, int color){
	U64 PawnPos = 1ULL << sq;
	U64 b = 0ULL;
		
	if (!color){ // White
		if (PawnPos & IsRank2){
			b |= PawnPos >> 16;
		}
		b |= PawnPos >> 8;
	}
	else { // Black		
		if (PawnPos & IsRank7){
			b |= PawnPos << 16;
		}
		b |= PawnPos << 8;
	}
	
	return b;
}

U64 KnightMoveBitboard(int sq){
	U64 KnightPos = 1ULL << sq;
	U64 b = 0ULL;	
	if (!(KnightPos & IsAFile) && !(KnightPos & IsRank78)){
		b |= KnightPos >> 17;
	}
	if (!(KnightPos & IsHFile) && !(KnightPos & IsRank78)){
		b |= KnightPos >> 15;
	}
	if (!(KnightPos & IsABFile) && !(KnightPos & IsRank8)){
		b |= KnightPos >> 10;
	}
	if (!(KnightPos & IsGHFile) && !(KnightPos & IsRank8)){
		b |= KnightPos >> 6;
	}
	if (!(KnightPos & IsAFile) && !(KnightPos & IsRank12)){
		b |= KnightPos << 15;
	}
	if (!(KnightPos & IsHFile) && !(KnightPos & IsRank12)){
		b |= KnightPos << 17;
	}
	if (!(KnightPos & IsABFile) && !(KnightPos & IsRank1)){
		b |= KnightPos << 6;
	}
	if (!(KnightPos & IsGHFile) && !(KnightPos & IsRank1)){
		b |= KnightPos << 10;
	}
	
	return b;
}

U64 KingMoveBitboard(int sq){
	U64 KingPos = 1ULL << sq;
	U64 b = 0ULL;
	
	// Left Right Up Down
	if (!(KingPos & IsAFile)){
		b |= KingPos >> 1;
	}
	if (!(KingPos & IsHFile)){
		b |= KingPos << 1;
	}
	if (!(KingPos & IsRank8)){
		b |= KingPos >> 8;
	}
	if (!(KingPos & IsRank1)){
		b |= KingPos << 8;
	}
	
	// Diagonal Moves
	if (!(KingPos & IsAFile) && !(KingPos & IsRank8)){ // Top Left
		b |= KingPos >> 9;
	}
	if (!(KingPos & IsHFile) && !(KingPos & IsRank8)){ // Top Right
		b |= KingPos >> 7;
	}
	if (!(KingPos & IsAFile) && !(KingPos & IsRank1)){ // Bottom Left
		b |= KingPos << 7;
	}
	if (!(KingPos & IsHFile) && !(KingPos & IsRank1)){ // Bottom Right
		b |= KingPos << 9;
	}
	
	return b;
}

void PrecomputePawnTables(U64 attacks[2][64], U64 moves[2][64]){
	for (int i=0; i<64; i++){
		attacks[white][i] = PawnAttackBitboard(i, white);
		attacks[black][i] = PawnAttackBitboard(i, black);
		moves[white][i] = PawnMoveBitboard(i, white);
		moves[black][i] = PawnMoveBitboard(i, black);
	}
}

void PrecomputeKnightTables(U64 moves[64]){
	for (int i=0; i<64; i++) moves[i] = KnightMoveBitboard(i);
}

void PrecomputeKingTables(U64 moves[64]){
	for (int i=0; i<64; i++) moves[i] = KingMoveBitboard(i);
}