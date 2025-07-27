#include "include/move.h"

static const U64 IsRank1 = 0xFF00000000000000;
static const U64 IsRank8 = 0x00000000000000FF;

void AddMove(MoveList* moves, int move){
	moves->moves[moves->count] = move;
	moves->count++;
}

void GenerateMoves(Board* b, MoveList* moves, int quiet){
	moves->count = 0;
	GeneratePawnMoves(b, moves, quiet);
}

// WARNING: Does not reset count on moves_list.
// It is meant to be part of the GenerateMoves function
void GeneratePawnMoves(Board* b, MoveList* moves, int quiet){
	U64 attacks;
	U64 bitboard = b->bitboards[b->color][pawn];
	U64 all_blockers = b->bitboards[0][6] | b->bitboards[1][6];

	int color = b->color;

	int source_sq, target_sq;

	while (bitboard){
		source_sq = GetLSBIndex(bitboard);
		PopBit(bitboard, source_sq);

		if (quiet){
			// Pawn Moves
			attacks = PrecomputedPawnMoves[color][source_sq];

			int bits = CountBits(attacks);

			if (bits == 1 && !(attacks & all_blockers)){ // If there is only single pawn pushes and it isn't blocked
				// Target square will be the same for promotion or regular move
				target_sq = GetLSBIndex(attacks);

				if ((1ULL << target_sq) & (color ? IsRank1 : IsRank8)){ // Promotion
					AddMove(moves, EncodeMove(source_sq, target_sq, EncodePiece(color, pawn), EncodePiece(color, knight), 0, 0, 0, 0));
					AddMove(moves, EncodeMove(source_sq, target_sq, EncodePiece(color, pawn), EncodePiece(color, bishop), 0, 0, 0, 0));
					AddMove(moves, EncodeMove(source_sq, target_sq, EncodePiece(color, pawn), EncodePiece(color, rook), 0, 0, 0, 0));
					AddMove(moves, EncodeMove(source_sq, target_sq, EncodePiece(color, pawn), EncodePiece(color, queen), 0, 0, 0, 0));
				}
				else {
					AddMove(moves, EncodeMove(source_sq, target_sq, EncodePiece(color, pawn), 0, 0, 0, 0, 0));
				}
			}
			else if (bits == 2) { // Double pawn push available, but also add single pawn push
				// This checks if there is a blocker on the single pawn push square
				if (!(all_blockers & (1ULL << (color ? (source_sq+8) : (source_sq-8))))){ // Single Pawn Push
					// Single Pawn Push
					target_sq = source_sq + (color ? 8 : -8);
					AddMove(moves, EncodeMove(source_sq, target_sq, EncodePiece(color, pawn), 0, 0, 0, 0, 0));
				}
				
				// If there are no blockers at all, then you can double pawn push
				if (!(attacks & all_blockers)){
					target_sq = source_sq + (color ? 16 : -16);
					AddMove(moves, EncodeMove(source_sq, target_sq, EncodePiece(color, pawn), 0, 0, 1, 0, 0));
				}
			}
		}

		// Pawn Attacks
		
		attacks = PrecomputedPawnAttacks[color][source_sq];
		
		attacks &= b->bitboards[OppColor(color)][6]; // Can't capture blank squares
		
		while (attacks){
			target_sq = GetLSBIndex(attacks);
			PopBit(attacks, target_sq);

			if ((1ULL << target_sq) & (color ? IsRank1 : IsRank8)){ // Promotion
				AddMove(moves, EncodeMove(source_sq, target_sq, EncodePiece(color, pawn), EncodePiece(color, knight), 1, 0, 0, 0));
				AddMove(moves, EncodeMove(source_sq, target_sq, EncodePiece(color, pawn), EncodePiece(color, bishop), 1, 0, 0, 0));
				AddMove(moves, EncodeMove(source_sq, target_sq, EncodePiece(color, pawn), EncodePiece(color, rook), 1, 0, 0, 0));
				AddMove(moves, EncodeMove(source_sq, target_sq, EncodePiece(color, pawn), EncodePiece(color, queen), 1, 0, 0, 0));
			}
			else {
				AddMove(moves, EncodeMove(source_sq, target_sq, EncodePiece(color, pawn), 0, 1, 0, 0, 0));
			}
		}

		// Add En passant
		if (b->en_sq != -1){
			// Shouldn't be necessary
			if (b->bitboards[OppColor(color)][pawn] & (1ULL << b->en_sq)){
				int offset = (color ? -8 : 8);
				attacks = PrecomputedPawnAttacks[color][source_sq];
		
				attacks &= ~all_blockers; // Has to be a blank square
				
				while (attacks){
					target_sq = GetLSBIndex(attacks);
					PopBit(attacks, target_sq);
					if ((target_sq+offset) == b->en_sq)
					AddMove(moves, EncodeMove(source_sq, target_sq, EncodePiece(color, pawn), 0, 1, 0, 1, 0));
				}
			}
		}
	}
}