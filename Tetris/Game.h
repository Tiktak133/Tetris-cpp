#pragma once
#include "Display.h"
#include "Pieces.h"
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

class Game {
public:
	double fallInterval;
	double fallTimer;

	Game(Display& display, Board& board);
	~Game();
	void MovePieceDown();
	void MovePieceLeft();
	void MovePieceRight();
	void RotatePiece();
	void DropPiece();
	bool IsGameOver() const { return gameOver_; }

private:
	Display& display_;
	Board& board_;
	int currentType_;
	Piece currentPiece_;
	bool gameOver_ = false;
	std::vector<int> bag_;
	int posX_;
	int posY_;
	std::mt19937 rng_; // generator liczb losowych
	void SpawnNewPiece();
	bool CanPlace(const Piece& piece, int x, int y) const;
	void UpdateLook(int oldX, int oldY);
	void FreezePiece();
};