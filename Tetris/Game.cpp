#include "Game.h"

Game::Game(Display& display, Board& board) : display_(display), board_(board), score_(display, 3, L"Punkty", 0), lines_(display, 4, L"Linie", 0), level_(display, 5, L"Poziom", 1)
{
	fallInterval_ = 1.0;   // klocek spada co 1.0 s
	fallTimer_ = 0;
	posX_ = 3;
	posY_ = 0;
	bag_ = std::vector<int>();
	rng_ = std::mt19937{ std::random_device{}() };

	SpawnNewPiece();
}

void Game::Update(double delta)
{
	if (gameOver_)
		return;

	fallTimer_ += delta;
	while (fallTimer_ >= fallInterval_ && !gameOver_)
	{
		fallTimer_ -= fallInterval_;
		MovePieceDown();
	}
}

void Game::SetFallInterval(double seconds)
{
	fallInterval_ = std::max(0.05, seconds); // nie szybciej niż 20 pól na sekundę
}

void Game::MovePieceDown()
{
	if (CanPlace(currentPiece_, posX_, posY_ + 1))
		UpdateLook(posX_, posY_++);
	else
		FreezePiece(); // niżej nie ma miejsca - klocek zostaje zamrożony
}

void Game::MovePieceLeft()
{
	if (CanPlace(currentPiece_, posX_ - 1, posY_))
		UpdateLook(posX_--, posY_);
}

void Game::MovePieceRight()
{
	if (CanPlace(currentPiece_, posX_ + 1, posY_))
		UpdateLook(posX_++, posY_);
}

void Game::RotatePiece() //90 stopni zgodnie z zegarem
{
	Piece rotated = currentPiece_;
	Rotate(rotated); // obracamy kopię - oryginał zostaje, jeśli obrót się nie zmieści

	// Korekta ("wall kick"): najpierw w miejscu, potem przesunięcie w bok.
	// +-2 jest potrzebne dla klocka I przy ścianie.
	for (int dx : { 0, -1, 1, -2, 2 })
	{
		if (CanPlace(rotated, posX_ + dx, posY_))
		{
			int oldX = posX_;
			currentPiece_ = rotated;
			posX_ += dx;
			UpdateLook(oldX, posY_);
			return;
		}
	}
	// żadna pozycja nie pasuje - obrót się nie wykonuje
}

void Game::DropPiece()
{
	display_.RestoreRect(posX_, posY_, currentPiece_.size, currentPiece_.size);
	int startY = posY_;
	while (CanPlace(currentPiece_, posX_, posY_ + 1))
		posY_++;
	score_ += 2 * (posY_ - startY);   // 2 punkty za każde pole zrzutu
	FreezePiece();
}

// --- Private ----

void Game::SpawnNewPiece()
{
	if (bag_.empty())
	{
		bag_ = { 0, 1, 2, 3, 4, 5, 6 };
		std::shuffle(bag_.begin(), bag_.end(), rng_);
	}
	currentType_ = bag_.back();
	currentPiece_ = PIECES[currentType_];
	bag_.pop_back();
	posX_ = 3;
	posY_ = 0;

	if (!CanPlace(currentPiece_, posX_, posY_))
	{
		gameOver_ = true;
		return;
	}

	UpdateLook(posX_, posY_);
}

// Czy klocek zmieści się na pozycji (x, y)? Sprawdza tylko zajęte kratki klocka:
// muszą być w granicach planszy i na pustym polu warstwy stałej.
bool Game::CanPlace(const Piece& piece, int x, int y) const
{
	for (int j = 0; j < piece.size; j++)
		for (int i = 0; i < piece.size; i++)
		{
			if (!piece.cells[j][i])
				continue; // puste pola macierzy nie mają znaczenia

			int bx = x + i;
			int by = y + j;
			if (bx < 0 || bx >= board_.Width() || by < 0 || by >= board_.Height())
				return false; // poza planszą
			if (board_.Get(bx, by) != 0)
				return false; // zajęte przez zamrożony klocek
		}
	return true;
}

void Game::UpdateLook(int oldX, int oldY)
{
	display_.RestoreRect(oldX, oldY, currentPiece_.size, currentPiece_.size);
	display_.DrawPieceAt(posX_, posY_, currentPiece_);
}

void Game::FreezePiece()
{
	for (int j = 0; j < currentPiece_.size; j++)
		for (int i = 0; i < currentPiece_.size; i++)
			if (currentPiece_.cells[j][i])
				board_.Set(posX_ + i, posY_ + j, currentType_ + 1);

	display_.DrawPieceAt(posX_, posY_, currentPiece_);

	int lowestRow = posY_ + currentPiece_.size - 1;
	int cleared = board_.ClearFullRows(posY_, lowestRow);
	if (cleared > 0)
	{
		display_.RestoreRect(0, 0, board_.Width(), std::min(lowestRow + 1, board_.Height()));

		const int points[5] = { 0, 100, 300, 500, 800 };
		score_ += points[cleared] * level_.Get();
		lines_ += cleared;
		level_ = 1 + lines_.Get() / 10;   // co 10 linii poziom wyżej

		int lv = level_.Get();
		SetFallInterval(std::pow(0.8 - (lv - 1) * 0.007, lv - 1));   // szybsze spadanie
	}

	SpawnNewPiece();
}