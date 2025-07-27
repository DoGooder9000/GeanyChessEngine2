#pragma once

#define U64 unsigned long long

#define GetBit(b, i) ((b) & (1ULL << (i))) // Usually returns a pretty large number if true, have to use a ? 1 : 0 to convert to 1 or 0
#define SetBit(b, i) ((b) |= (1ULL << (i)))
#define ClearBit(b, i) ((b) &= ~(1 << (i)))
#define PopBit(b, i) ((b) &= ~(1ULL << (i)))