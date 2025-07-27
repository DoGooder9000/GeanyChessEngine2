#include "include/board.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

void ParseFEN(Board* b, char* FEN){
	int len = strlen(FEN);

	char c;
	int b_pos = 0;
	int i;
	for (i=0; i<len; i++){
		c = FEN[i];

		if (c == ' ') {i++; break;} // Goto next section
		if (c == '/') continue; // Skip over these because the board is one dimensional
		
		if (isdigit(c)){
			for (int j=0; j<(c - '0'); j++){
				b->pieces[b_pos] = -1;
				b_pos++;
			}
			continue;
		}
		else{
			switch (c){
				case 'P':
					b->pieces[b_pos] = EncodePiece(white, pawn);
					break;

				case 'N':
					b->pieces[b_pos] = EncodePiece(white, knight);
					break;
				
				case 'B':
					b->pieces[b_pos] = EncodePiece(white, bishop);
					break;
				
				case 'R':
					b->pieces[b_pos] = EncodePiece(white, rook);
					break;

				case 'Q':
					b->pieces[b_pos] = EncodePiece(white, queen);
					break;
				
				case 'K':
					b->pieces[b_pos] = EncodePiece(white, king);
					break;
				
				case 'p':
					b->pieces[b_pos] = EncodePiece(black, pawn);
					break;

				case 'n':
					b->pieces[b_pos] = EncodePiece(black, knight);
					break;
				
				case 'b':
					b->pieces[b_pos] = EncodePiece(black, bishop);
					break;
				
				case 'r':
					b->pieces[b_pos] = EncodePiece(black, rook);
					break;

				case 'q':
					b->pieces[b_pos] = EncodePiece(black, queen);
					break;
				
				case 'k':
					b->pieces[b_pos] = EncodePiece(black, king);
					break;
			}
			b_pos++;
		}
	}

	// Now the color to move
	if (FEN[i] == 'w') b->color = white;
	else b->color = black;
	i += 2; // skip the w or b and skip the ' '
	
	// Now castling
	c = FEN[i];
	b->castle_rights = 0;
	while (c != ' ' && i < len){
		if (c == '-'){
			i++; // Get to the space
			break;
		}
		if (c == 'K'){
			b->castle_rights |= WK;
		}
		else if (c == 'Q'){
			b->castle_rights |= WQ;
		}
		else if (c == 'k'){
			b->castle_rights |= BK;
		}
		else if (c == 'q'){
			b->castle_rights |= BQ;
		}
		
		i++;
		c = FEN[i];
	}
	
	i++; // Get the next thing after the space
	
	// En passant
	if (FEN[i] == '-'){
		b->en_sq = -1;
		i += 2; // Get the next part
	}
	else {
		b->en_sq = GetIntPos(FEN+i); // Should return square number based on readable coordinate
		i += 3; // Because the square name will be 2 characters long + the space
	}
	
	// Half moves
	if (isdigit(FEN[i])){
		// Check if it is single or double digit
		if (isdigit(FEN[i+1])){
			// It is a double digit
			int num1 = FEN[i] - '0';
			int num2 = FEN[i+1] - '0';
			
			num1 *= 10;
			b->half_moves = num1+num2;
			
			i++;
		}
		else {
			// It is single digit
			b->half_moves = FEN[i] - '0';
		}
	}
	
	i += 2; // Go past the number and the space
	
	// Full Moves, Handles up to 3 digits
	if (isdigit(FEN[i])){
		int num1 = FEN[i] - '0';
		
		if (isdigit(FEN[i+1])){
			int num2 = FEN[i+1] - '0';
			
			if (isdigit(FEN[i+2])){
				int num3 = FEN[i+2] - '0';
				
				b->full_moves = num1*100 + num2*10 + num3;
			}
			else b->full_moves = num1*10 + num2;
		}
		else b->full_moves = num1;
	}
}

void GenerateBitboards(Board* b){
	for (int i=0; i<14; i++) b->bitboards[i/7][i%7] = 0ULL;
	
	for (int i=0; i<64; i++){
		// Also need to update the same color bitboard [6]
		if (b->pieces[i] != -1){
			b->bitboards[PieceColor(b->pieces[i])][PieceType(b->pieces[i])] |= (1ULL << i);
			b->bitboards[PieceColor(b->pieces[i])][6] |= (1ULL << i);
		}
	}
}

int GetIntPos(char* s){
		int pos;
		switch (s[0]){
				case 'a':
					pos = 0;
					break;
				case 'b':
					pos = 1;
					break;
				case 'c':
					pos = 2;
					break;
				case 'd':
					pos = 3;
					break;
				case 'e':
					pos = 4;
					break;
				case 'f':
					pos = 5;
					break;
				case 'g':
					pos = 6;
					break;
				case 'h':
					pos = 7;
					break;
		
				default:
					return -1;
		}
		
		if (!isdigit(s[1])){ return -1; }
		
		pos += (8-(s[1]-'0'))*8;
		
		return pos;
}

int isSquareAttacked(Board* b, int sq, int color){
	U64 all_blockers = b->bitboards[0][6] | b->bitboards[1][6];
	
	// If it is attacked by pawns
	// We get the opposite color
	if (PrecomputedPawnAttacks[OppColor(color)][sq] & b->bitboards[color][pawn]) return 1;
	if (PrecomputedKnightMoves[sq] & b->bitboards[color][knight]) return 1;
	if (PrecomputedKingMoves[sq] & b->bitboards[color][king]) return 1;
	/* if (GetBishopAttacks(sq, all_blockers) & b->bitboards[color][bishop]) return 1;
	if (GetRookAttacks(sq, all_blockers) & b->bitboards[color][rook]) return 1;
	if (GetQueenAttacks(sq, all_blockers) & b->bitboards[color][queen]) return 1; */

	return 0;
}