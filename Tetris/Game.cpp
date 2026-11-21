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
	UpdateLook(posX_, posY_++);
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
	display_.FillRect(posX_, posY_, currentPiece_.size, currentPiece_.size, 0, 0, 0);
	posY_ = display_.getHeight() - calcHeightOfPiece(false);
	display_.DrawPieceAt(posX_, posY_, currentPiece_);
}

void Game::UpdateLook(int oldX, int oldY)
{
	display_.FillRect(oldX, oldY, currentPiece_.size, currentPiece_.size, 0, 0, 0);
	display_.DrawPieceAt(posX_, posY_, currentPiece_);
}

// --- Private ----

void Game::SpawnNewPiece()
{
	if (bag_.empty())
	{
		bag_ = { 0, 1, 2, 3, 4, 5, 6 };
		std::shuffle(bag_.begin(), bag_.end(), rng_);
	}
	currentPiece_ = PIECES[bag_.back()];
	bag_.pop_back();
	posX_ = 3;
	posY_ = 0;

	UpdateLook(posX_, posY_);
}

int Game::calcHeightOfPiece(const bool ifRotate)
{
	int minY = 4;
	int maxY = -1;

	for (int y = 0; y < currentPiece_.size; y++)
		for (int x = 0; x < currentPiece_.size; x++)
			if (currentPiece_.cells[y][x])
			{
				int row = ifRotate ? x : y;
				minY = std::min(minY, row);
				maxY = std::max(maxY, row);
			}

	return maxY - minY + 1;
}