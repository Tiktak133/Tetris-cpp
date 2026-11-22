#pragma once
#include <vector>

class Board
{
public:
	Board(int width, int height);

	int Width() const { return width_; }
	int Height() const { return height_; }

	unsigned char Get(int x, int y) const;
	void Set(int x, int y, unsigned char v);

	bool IsRowFull(int y) const;

	// Usuwa pełne wiersze. Sprawdza tylko zakres fromY..toY (tam, gdzie wylądował klocek).
	// Zwraca liczbę usuniętych wierszy.
	int ClearFullRows(int fromY, int toY);

private:
	int width_;
	int height_;
	std::vector<unsigned char> cells_;
};