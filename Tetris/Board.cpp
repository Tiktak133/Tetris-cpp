#include "Board.h"
#include <algorithm>
#include <stdexcept>

Board::Board(int width, int height) : width_(width), height_(height)
{
	// konstruktor nie może zwrócić błędu - zostaje wyjątek
	if (width <= 0 || height <= 0)
		throw std::invalid_argument("Rozmiar planszy musi być większy od zera");

	cells_.assign(width * height, 0);
}

unsigned char Board::Get(int x, int y) const
{
	// bez sprawdzenia odczytalibyśmy cudzą pamięć
	if (x < 0 || x >= width_ || y < 0 || y >= height_)
		throw std::out_of_range("Board::Get - współrzędne poza planszą");

	return cells_[y * width_ + x];
}

void Board::Set(int x, int y, unsigned char v)
{
	// bez sprawdzenia nadpisalibyśmy cudzą pamięć
	if (x < 0 || x >= width_ || y < 0 || y >= height_)
		throw std::out_of_range("Board::Set - współrzędne poza planszą");

	cells_[y * width_ + x] = v;
}

bool Board::IsRowFull(int y) const
{
	for (int x = 0; x < width_; x++)
		if (Get(x, y) == 0)
			return false;
	return true;
}

int Board::ClearFullRows(int fromY, int toY)
{
	fromY = std::max(fromY, 0);
	toY = std::min(toY, height_ - 1);

	bool any = false;
	for (int y = fromY; y <= toY; y++)
		if (IsRowFull(y))
			any = true;
	if (!any)
		return 0;                  // najczęstszy przypadek - nic do roboty

	int dst = toY;                 // wiersze poniżej toY się nie zmieniają
	for (int src = toY; src >= 0; src--)
	{
		if (src >= fromY && IsRowFull(src))
			continue;              // pełny - pomijamy, czyli usuwamy
		if (dst != src)
			std::copy(cells_.begin() + src * width_, cells_.begin() + (src + 1) * width_, cells_.begin() + dst * width_);
		dst--;
	}
	std::fill(cells_.begin(), cells_.begin() + (dst + 1) * width_, 0); // puste wiersze na górze
	return dst + 1;
}