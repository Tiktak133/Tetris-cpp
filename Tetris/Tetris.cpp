#include <iostream>
#include "Display.h"
#include <chrono>
#include <thread>
#include <conio.h>
#include "Action.h"
#include "Game.h"
#include "Board.h"
#include "Stats.h"
#include <stdexcept>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#undef DrawText // makro z windows.h koliduje z Display::DrawText

using Clock = std::chrono::steady_clock;

// Komunikat wyjątku (e.what()) to tekst UTF-8 w char*, a konsola pisze znakami szerokimi (wchar_t)
std::wstring Utf8ToWide(const char* text)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, text, -1, nullptr, 0);
	std::wstring result(len > 0 ? len - 1 : 0, L'\0');
	if (len > 1)
		MultiByteToWideChar(CP_UTF8, 0, text, -1, result.data(), len);
	return result;
}

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

int RunGame()
{
	static const int width = 10;
	static const int height = 20;

	Board board(width, height);
	Display display(board);

	// --- ekran powitalny ---
	display.ShowWelcome();
	_getch();                        // czekamy na dowolny klawisz
	while (_kbhit()) _getch();       // strzałka to 2 kody - wyrzucamy resztę, żeby nie ruszyła klocka
	display.DrawScreen();

	TextStat title(display, 0, L"-=- TETRIS -=-");
	FloatStat time(display, 1, L"Czas", 0.0);

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

		game.Update(dt);
		time += dt;

		// limit FPS (żeby nie zjadać 100% CPU)
		auto elapsed = Clock::now() - frameStart;
		if (elapsed < frameTime)
			std::this_thread::sleep_for(frameTime - elapsed);
	}

	std::wcout << (game.IsGameOver() ? L"Koniec gry!\n" : L"Do zobaczenia\n");
	std::vector<const Stat*> summary = game.GetStats();
	summary.insert(summary.begin(), &time);
	for (const Stat* s : summary)
		std::wcout << L"  " << s->Text() << L"\n";

	std::wcout << L"\nNaciśnij Enter, aby zamknąć...";
	while (_getch() != 13) {}   // 13 = Enter; inne klawisze (np. przytrzymana strzałka) są ignorowane

    return 0;
}

int main()
{
	try
	{
		return RunGame();
	}
	catch (const std::exception& e)
	{
		(void)_setmode(_fileno(stderr), _O_U16TEXT);
		std::wcerr << L"\033[0m\033[?25h\n"           // reset kolorów i widoczny kursor
		           << L"Błąd: " << Utf8ToWide(e.what()) << L"\n";
		return 1;
	}
}