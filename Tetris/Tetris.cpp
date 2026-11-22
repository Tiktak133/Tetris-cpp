#include <iostream>
#include "Display.h"
#include <chrono>
#include <thread>
#include <conio.h>
#include "Action.h"
#include "Game.h"
#include "Board.h"
#include "Stats.h"	

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

int main() 
{
	static const int width = 10;
	static const int height = 20;

	Board board(width, height);
	Display display(board);

	// --- ekran powitalny ---
	display.ShowWelcome();
	_getch();                        // czekamy na dowolny klawisz
	while (_kbhit()) _getch();       // strzałka to 2 kody - wyrzucamy resztę, żeby nie ruszyła klocka
	display.DrawScreen();            // z powrotem plansza

	TextStat title(display, 0, L"-=- TETRIS -=-");
	FloatStat time(display, 1, L"Czas: ", 0.0);

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