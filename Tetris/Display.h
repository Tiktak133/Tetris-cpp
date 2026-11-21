#pragma once
#include <iostream>
#include <fcntl.h>
#ifdef _WIN32
#include <io.h>
#endif
#include <string>
#include "Pieces.h"

class Display
{
public:
	Display(int width, int height);
	void DrawScreen();
	void DrawPixelAt(int x, int y, int r, int g, int b);
	void FillRect(int x, int y, int width, int height, int r, int g, int b);
	void DrawPieceAt(int x, int y, const struct Piece& piece);
	void Clear();
	int getWidth() const { return width_; }
	int getHeight() const { return height_; }
	~Display();

private:
	int width_;
	int height_;
	int y_start_ = 0;
	void InitSysCompatible();
};