#pragma once

struct Color { int r, g, b; };

struct Piece
{
	int size;
	int cells[4][4];
	Color color;
};

const Piece PIECES[7] =
{
	{ 4, { {0,0,0,0},
		   {1,1,1,1},
		   {0,0,0,0},
		   {0,0,0,0} }, {  40, 200, 230 } }, // I – cyjan

	{ 2, { {1,1},
		   {1,1} },     { 245, 210,  50 } }, // O – żółty

	{ 3, { {0,1,0},
		   {1,1,1},
		   {0,0,0} },   { 170,  80, 230 } }, // T – fiolet

	{ 3, { {0,1,1},
		   {1,1,0},
		   {0,0,0} },   {  90, 205,  80 } }, // S – zielony

	{ 3, { {1,1,0},
		   {0,1,1},
		   {0,0,0} },   { 230,  70,  80 } }, // Z – czerwony

	{ 3, { {1,0,0},
		   {1,1,1},
		   {0,0,0} },   {  60, 110, 235 } }, // J – niebieski

	{ 3, { {0,0,1},
		   {1,1,1},
		   {0,0,0} },   { 245, 145,  50 } }, // L – pomarańczowy
};

inline void Rotate(Piece& p) // 90 stopni zgodnie ze wskazówkami zegarem
{
	int tmp[4][4] = {};
	for (int y = 0; y < p.size; y++)
		for (int x = 0; x < p.size; x++)
			tmp[x][p.size - 1 - y] = p.cells[y][x];

	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
			p.cells[y][x] = tmp[y][x];
}