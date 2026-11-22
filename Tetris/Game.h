#pragma once
#include "Display.h"
#include "Pieces.h"
#include "Stats.h"
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include <cmath>

class Game {
public:
	Game(Display& display, Board& board);
	void Update(double delta);
	void MovePieceDown();
	void MovePieceLeft();
	void MovePieceRight();
	void RotatePiece();
	void DropPiece();

	// --- gettery ---
	bool IsGameOver() const { return gameOver_; }
	std::vector<const Stat*> GetStats() const { return { &score_, &lines_, &level_ }; } // wszystkie statystyki gry

	// --- settery ---
	void SetFallInterval(double seconds); // pilnuje minimalnej wartości

private:
	double fallInterval_;            // co ile sekund klocek spada o 1 pole
	double fallTimer_;               // ile czasu minęło od ostatniego kroku w dół
	Display& display_;
	Board& board_;

	IntStat score_;
	IntStat lines_;
	IntStat level_;

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