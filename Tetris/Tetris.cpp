#include <iostream>
#include "Display.h"
#include <chrono>
#include <thread>
#include <conio.h>
#include "Action.h"
#include "Game.h"
#include "Board.h"

using Clock = std::chrono::steady_clock;

void Inputs(Game& game, const Action& action)
{
	switch (action)
	{
		case Action::Left:   game.MovePieceLeft(); break;
		case Action::Right:  game.MovePieceRight(); break;
		case Action::Down:   game.DropPiece(); break;
		case Action::Rotate: game.RotatePiece(); break;
		default: break;
	}
}

void Update(Game& game, const double& delta)
{
	game.fallTimer += delta;                          
	while (game.fallTimer >= game.fallInterval)    
	{
		game.fallTimer -= game.fallInterval;       
		game.MovePieceDown();
	}
}

void Render(Display& display)
{
	//display.DrawStats();
}

int main() 
{
	static const int width = 10;
	static const int height = 20;

	Board board(width, height);
	Display display(board);
	Game game = Game(display, board);

    const auto frameTime = std::chrono::milliseconds((int)(1000.0 / 30.0)); // ~30 FPS

    auto last = Clock::now();
    bool running = true;

	while (running && !game.IsGameOver())
	{
		// --- delta ---
		auto frameStart = Clock::now();
		double dt = std::chrono::duration<double>(frameStart - last).count(); // w sekundach
		last = frameStart;
		if (dt > 0.25) dt = 0.25; // np. po pauzie w debuggerze – bez nagłego skoku

		while (_kbhit())
		{
			Action a = ReadAction();
			if (a == Action::Quit) running = false;
			else Inputs(game, a);
		}

		Update(game, dt);
		Render(display);

		// limit FPS (żeby nie zjadać 100% CPU)
		auto elapsed = Clock::now() - frameStart;
		if (elapsed < frameTime)
			std::this_thread::sleep_for(frameTime - elapsed);
	}

	if (game.IsGameOver())
		std::wcout << L"Koniec gry!\n";
	else
		std::wcout << L"Do zobaczenia\n";
    return 0;
}


/*
// Wypisujemy jakiś stały interfejs tekstowy, który ma stać w miejscu
std::wcout << L"=== SILNIK GRAFICZNY W KONSOLI ===\n";
std::wcout << L"Punkty: 1500\n";
std::wcout << L"----------------------------------\n";

// Rysujemy piksele w losowych miejscach, kompletnie ignorując tekst powyżej
drawPixelAt(0, 0, 255, 0, 0);
drawPixelAt(1, 1, 0, 255, 0);
drawPixelAt(2, 2, 0, 0, 255);
drawPixelAt(10, 20, 255, 255, 255);

// Na koniec przesuwamy kursor bezpiecznie na dół, żeby tekst systemowy niczego nie zamazal
//std::wcout << L"\033[12;1H";

//std::wcout << L"\033[38;2;255;0;0m" << L"░░" << L" - 25%\n";
//std::wcout << L"\033[38;2;0;255;0m" << L"▒▒" << L" - 50%\n";
//std::wcout << L"\033[38;2;0;0;255m" << L"▓▓" << L" - 75%\n";
//std::wcout << L"\033[38;2;255;255;0m" << L"██" << L" - 100%\n\n";
*/