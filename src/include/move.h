#pragma once

#include "board.h"
#include "bitboard.h"

#define SRC_SQ_MASK			0x00003F // 0b0000 0000 0000 0000 0011 1111
#define TGT_SQ_MASK			0x000FC0 // 0b0000 0000 0000 1111 1100 0000
#define PIECE_MASK			0x00F000 // 0b0000 0000 1111 0000 0000 0000
#define PR_PIECE_MASK	 	0x0F0000 // 0b0000 1111 0000 0000 0000 0000
#define CAPTURE_MASK		0x100000 // 0b0001 0000 0000 0000 0000 0000
#define DOUBLE_MASK			0x200000 // 0b0010 0000 0000 0000 0000 0000
#define EN_PAS_MASK			0x400000 // 0b0100 0000 0000 0000 0000 0000
#define CASTLE_MASK			0x800000 // 0b1000 0000 0000 0000 0000 0000

#define EncodeMove(src, tgt, piece, pro_piece, is_cap, is_double, is_en_pass, is_castle) \
		((src))				 	|		\
		((tgt) << 6)			|		\
		((piece) << 12)			|		\
		((pro_piece) << 16)		|		\
		((is_cap) << 20)		|		\
		((is_double) << 21)		|		\
		((is_en_pass) << 22)	|		\
		((is_castle) << 23)

#define GetSrcSq(move)		(move & SRC_SQ_MASK)
#define GetTgtSq(move)		((move & TGT_SQ_MASK) >> 6)
#define GetPiece(move)		((move & PIECE_MASK) >> 12)
#define GetPrPiece(move)	((move & PR_PIECE_MASK) >> 16)
#define GetCapture(move)	((move & CAPTURE_MASK) >> 20)
#define GetDouble(move)		((move & DOUBLE_MASK) >> 21)
#define GetEnPass(move)		((move & EN_PAS_MASK) >> 22)
#define GetCastle(move)		((move & CASTLE_MASK) >> 23)

void MakeMove(Board* b, int move);
void UnMakeMove(Board* b, int move);

void AddMove(MoveList* moves, int move);

void GenerateMoves(Board* b, MoveList* moves, int quiet);
void GeneratePawnMoves(Board* b, MoveList* moves, int quiet);