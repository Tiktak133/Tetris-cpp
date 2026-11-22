#pragma once
#include <iostream>
#include <fcntl.h>
#ifdef _WIN32
#include <io.h>
#endif
#include <string>
#include "Pieces.h"
#include "Board.h"

class Display
{
public:
	Display(Board& board);
	void DrawScreen();
	void DrawPixelAt(int x, int y, int r, int g, int b);
	void FillRect(int x, int y, int width, int height, int r, int g, int b);
	void DrawPieceAt(int x, int y, const struct Piece& piece);
	void Clear();
	void RestoreRect(int x, int y, int width, int height);
	unsigned char GetCell(int x, int y) const;

	// Panel z prawej strony planszy. row = wiersz liczony od górnej krawędzi planszy (0, 1, 2...)
	void DrawText(int row, const std::wstring& text);
	void DrawStat(int row, const std::wstring& label, int value);
	void DrawStat(int row, const std::wstring& label, double value); // float też tu trafi
	int getWidth() const { return width_; }
	int getHeight() const { return height_; }
	~Display();

private:
	int width_;
	int height_;
	int y_start_ = 0;
	Board& board_;
	void InitSysCompatible();
};