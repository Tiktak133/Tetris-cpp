#include "Display.h"
#include <format>

Display::Display(Board& board) : board_(board) // referencję trzeba ustawić tutaj, nie w ciele konstruktora
{
	width_ = board.Width();
	height_ = board.Height();
	InitSysCompatible();
}

void Display::InitSysCompatible() 
{
#ifdef _WIN32
	(void)_setmode(_fileno(stdout), _O_U16TEXT); // Konfiguracja konsoli pod Visual Studio / Windows
#else
	try {
		std::locale::global(std::locale("C.UTF-8"));
	}
	catch (...) {
		std::locale::global(std::locale("")); // locale z systemu (np. pl_PL.UTF-8)
	}
	std::wcout.imbue(std::locale());
#endif
	std::wcout << L"\033[2J\033[H"; // Wyczyszczenie ekranu na starcie
	std::wcout << L"\033[?25l";     // Ukrycie kursora
}

void Display::DrawScreen()
{
	const wchar_t H = L'\u2550';	// ═
	const wchar_t V = L'\u2551';	// ║
	const wchar_t TL = L'\u2554';	// ╔
	const wchar_t TR = L'\u2557';	// ╗
	const wchar_t BL = L'\u255A';	// ╚
	const wchar_t BR = L'\u255D';	// ╝

	std::wstring border(width_ * 2, H);
	std::wcout << L"\033[2J\033[H"; // czysty ekran i kursor w lewy górny róg (np. po ekranie powitalnym)
	std::wcout << TL << border << TR << L'\n';
	for (int i = 0; i < height_; i++)
		std::wcout << V << std::wstring(width_ * 2, L' ') << V << L'\n';
	std::wcout << BL << border << BR << L'\n';

	Clear();
}

void Display::DrawPixelAt(int x, int y, int r, int g, int b)
{
	// 1. Skaczemy kursorem na pozycję Y, X (uwaga: w ANSI podaje się Y, potem X, indeksowane od 1)
	std::wcout << L"\033[" << (y + 2) << L";" << (x * 2 + 2) << L"H";
	// 2. Ustawiamy kolor tła i wypisujemy dwie spacje (nasz kwadratowy piksel)
	std::wcout << L"\033[48;2;" << r << L";" << g << L";" << b << L"m  ";
	// 3. Resetujemy kolory, żeby nie zepsuć reszty konsoli
	std::wcout << L"\033[0m";
	std::wcout << L"\033[" << (height_ + 3) << L";1H";
}

void Display::FillRect(int x, int y, int width, int height, int r, int g, int b)
{
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
			DrawPixelAt(x + i, y + j, r, g, b);
}

void Display::DrawPieceAt(int x, int y, const struct Piece& piece)
{
	for (int j = 0; j < piece.size; j++)
		for (int i = 0; i < piece.size; i++)
			if (piece.cells[j][i])
				DrawPixelAt(x + i, y + j, piece.color.r, piece.color.g, piece.color.b);
}

void Display::Clear()
{
	FillRect(0, 0, width_, height_, 0, 0, 0);
}

// Odtwarza prostokąt z warstwy stałej (board_): pusta kratka = czarna, zajęta = kolor swojego klocka
void Display::RestoreRect(int x, int y, int width, int height)
{
	for (int j = y; j < y + height; j++)
		for (int i = x; i < x + width; i++)
		{
			if (i < 0 || i >= width_ || j < 0 || j >= height_)
				continue; // poza planszą - nie rysujemy (nie zamaże ramki)

			unsigned char value = board_.Get(i, j);
			if (value == 0)
				DrawPixelAt(i, j, 0, 0, 0);
			else
			{
				const Color& c = PIECES[value - 1].color;
				DrawPixelAt(i, j, c.r, c.g, c.b);
			}
		}
}

void Display::ShowWelcome()
{
	const std::wstring bold = L"\033[1m";
	const std::wstring reset = L"\033[0m";

	std::wcout << L"\033[2J\033[H\n";

	// tytuł: każda litera w kolorze innego klocka
	const std::wstring title = L"T E T R I S";
	std::wcout << L"   " << bold;
	for (size_t i = 0, piece = 0; i < title.size(); i++)
	{
		if (title[i] == L' ')
		{
			std::wcout << L' ';
			continue;
		}
		const Color& c = PIECES[piece++ % 7].color;
		std::wcout << L"\033[38;2;" << c.r << L";" << c.g << L";" << c.b << L"m" << title[i];
	}
	std::wcout << reset << L"\n\n";

	std::wcout
		<< L"   Układaj spadające klocki tak, aby tworzyły pełne poziome linie.\n"
		<< L"   Pełna linia znika, a Ty dostajesz punkty.\n\n"

		<< L"   " << bold << L"STEROWANIE" << reset << L"\n"
		<< L"     A / ←     ruch w lewo\n"
		<< L"     D / →     ruch w prawo\n"
		<< L"     W / ↑     obrót\n"
		<< L"     S / ↓     zrzut na dół\n"
		<< L"     Esc       wyjście\n\n"

		<< L"   " << bold << L"PUNKTACJA" << reset << L" (mnożona przez poziom)\n"
		<< L"     1 linia   100\n"
		<< L"     2 linie   300\n"
		<< L"     3 linie   500\n"
		<< L"     4 linie   800   TETRIS!\n"
		<< L"     zrzut     2 pkt za każde pole\n\n"

		<< L"   " << bold << L"POZIOMY" << reset << L"\n"
		<< L"     Co 10 linii poziom rośnie, a klocki spadają szybciej.\n"
		<< L"     Gra kończy się, gdy nowy klocek nie ma już miejsca.\n\n"

		<< L"   " << bold << L"Naciśnij dowolny klawisz, aby zacząć..." << reset;
}

void Display::DrawText(int row, const std::wstring& text)
{
	// kolumna: za prawą ramką planszy + 2 spacje odstępu
	std::wcout << L"\033[" << (row + 2) << L";" << (width_ * 2 + 5) << L"H";
	// \033[K czyści resztę linii - gdy nowy tekst jest krótszy od starego, nie zostaną śmieci
	std::wcout << text << L"\033[K";
	std::wcout << L"\033[" << (height_ + 3) << L";1H";
}

void Display::DrawStat(int row, const std::wstring& label, int value)
{
	DrawText(row, label + L": " + std::to_wstring(value));
}

void Display::DrawStat(int row, const std::wstring& label, double value)
{
	DrawText(row, label + L": " + std::format(L"{:.2f}", value)); // 2 miejsca po przecinku
}

Display::~Display()
{
	std::wcout << L"\033[?25h"; // Przywrócenie kursora po wyjściu z gry
}