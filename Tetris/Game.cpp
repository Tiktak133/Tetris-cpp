#include "Game.h"

Game::Game(Display& display) : display_(display) 
{
	fallInterval = 1.0;   // klocek spada co 1.0 s
	fallTimer = 0;
	posX_ = 3;
	posY_ = 0;
	bag_ = std::vector<int>();
	rng_ = std::mt19937{ std::random_device{}() };

	SpawnNewPiece();
}

Game::~Game()
{

}



void Game::MovePieceDown()
{
	if (posY_ < display_.getHeight() - calcHeightOfPiece(false))
		UpdateLook(posX_, posY_++);
	else
		FreezePiece();
}

void Game::MovePieceLeft()
{
	if (posX_ > 0)
		UpdateLook(posX_--, posY_);
}

void Game::MovePieceRight()
{
	if (posX_ < display_.getWidth() - currentPiece_.size)
		UpdateLook(posX_++, posY_);
}

void Game::RotatePiece() //90 stopni zgodnie z zegarem
{
	Rotate(currentPiece_);
	UpdateLook(posX_, posY_);
}

void Game::DropPiece()
{
	display_.RestoreRect(posX_, posY_, currentPiece_.size, currentPiece_.size);
	posY_ = display_.getHeight() - calcHeightOfPiece(false);
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

	UpdateLook(posX_, posY_);
}

int Game::calcHeightOfPiece(const bool ifRotate)
{
	int maxY = -1;

	for (int y = 0; y < currentPiece_.size; y++)
		for (int x = 0; x < currentPiece_.size; x++)
			if (currentPiece_.cells[y][x])
				maxY = std::max(maxY, ifRotate ? x : y);

	return maxY + 1;
}

void Game::UpdateLook(int oldX, int oldY)
{
	display_.RestoreRect(oldX, oldY, currentPiece_.size, currentPiece_.size);
	display_.DrawPieceAt(posX_, posY_, currentPiece_);
}

void Game::FreezePiece()
{
	display_.FreezePieceAt(posX_, posY_, currentPiece_, currentType_ + 1);
	SpawnNewPiece();
}