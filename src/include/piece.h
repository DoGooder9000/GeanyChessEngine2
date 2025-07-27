#pragma once

#include "board.h"

// Layout
// 0111 Type
// 1000 Color

#define ColorMask 0b1000
#define TypeMask 0b0111

// Color 0-1, Type 0-5
#define MakePiece(color, type) ((color)<<3 | type)
#define PieceColor(piece) (((piece) & ColorMask) >> 3)
#define PieceType(piece) ((piece) & TypeMask)