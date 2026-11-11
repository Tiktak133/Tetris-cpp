#pragma once

class Display
{
public:
	Display(int width, int height);
	void DrawScreen();
	void DrawPixelAt(int x, int y, int r, int g, int b);
	~Display();

private:
	int width_;
	int height_;
	int y_start_ = 0;
	void InitSysCompatible();
};