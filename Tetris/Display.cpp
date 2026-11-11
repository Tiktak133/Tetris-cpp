#include "Display.h"
#include <iostream>
#include <fcntl.h>
#ifdef _WIN32
#include <io.h>
#endif
#include <string>

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
}

void Display::DrawPixelAt(int x, int y, int r, int g, int b)
{
	// 1. Skaczemy kursorem na pozycję Y, X (uwaga: w ANSI podaje się Y, potem X, indeksowane od 1)
	std::wcout << L"\033[" << (y + 1) << L";" << (x * 2 + 1) << L"H";
	// 2. Ustawiamy kolor tła i wypisujemy dwie spacje (nasz kwadratowy piksel)
	std::wcout << L"\033[48;2;" << r << L";" << g << L";" << b << L"m  ";
	// 3. Resetujemy kolory, żeby nie zepsuć reszty konsoli
	std::wcout << L"\033[0m";
}

Display::~Display()
{
	
}