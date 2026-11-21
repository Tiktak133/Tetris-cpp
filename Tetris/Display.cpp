#include "Display.h"

Display::Display(int width, int height)
{
	width_ = width;
	height_ = height;
	InitSysCompatible();
	DrawScreen();
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

Display::~Display()
{
	
}