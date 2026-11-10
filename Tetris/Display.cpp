#include <iostream>
#include <fcntl.h>
#include <io.h>

class Display
{
public:
	Display(int x, int y);
	void Start();
	void DrawScreen();
	void DrawPixelAt(int x, int y, int r, int g, int b);
	~Display();

private:
	int  width_;
	int height_;
	int y_start_ = 0;
	static constexpr const wchar_t* pixelStrength[] = { L"  ", L"░░", L"▒▒", L"▓▓", L"██"};
};

void Display::Start()
{
	
}

void Display::DrawScreen()
{
	
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

Display::Display(int x, int y)
{
	width_ = x;
	height_ = y;
}

Display::~Display()
{

}

enum pixelState
{
	empty,
	light,
	medium,
	dense,
	full
};