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

void Game::MovePieceDown()
{
	// Tutaj logika przesuwania klocka w dół
	std::wcout << L"Klocek spada w dół\n";
}

void Game::MovePieceLeft()
{
	// Tutaj logika przesuwania klocka w lewo
	std::wcout << L"Klocek w lewo\n";
}

void Game::MovePieceRight()
{
	// Tutaj logika przesuwania klocka w prawo
	std::wcout << L"Klocek w prawo\n";
}

void Game::RotatePiece() //90 stopni zgodnie z zegarem
{
	// Tutaj logika obracania klocka
	std::wcout << L"Obrót klocka\n";
}

void Game::DropPiece()
{
	// Tutaj logika upuszczania klocka na dół
	std::wcout << L"Klocek upuszczony na dół\n";
}

// --- Private ----

void Game::SpawnNewPiece()
{
	// Tutaj logika generowania nowego klocka
	std::wcout << L"Nowy klocek pojawił się\n";

	if (bag_.empty())
	{
		bag_ = { 0, 1, 2, 3, 4, 5, 6 };
		std::shuffle(bag_.begin(), bag_.end(), rng_);
	}
	currentPiece_ = PIECES[bag_.back()];
	bag_.pop_back();
	posX_ = 3;
	posY_ = 0;
}

Game::~Game()
{

}