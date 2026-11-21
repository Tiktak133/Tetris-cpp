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

	Game(Display& display);
	~Game();
	void MovePieceDown();
	void MovePieceLeft();
	void MovePieceRight();
	void RotatePiece();
	void DropPiece();

private:
	Display& display_;
	int currentType_;
	Piece currentPiece_;
	void SpawnNewPiece();
	std::vector<int> bag_;
	int posX_;
	int posY_;
	std::mt19937 rng_; // generator liczb losowych
	int calcHeightOfPiece(const bool ifRotate = false);
	void UpdateLook(int oldX, int oldY);
	void FreezePiece();
};