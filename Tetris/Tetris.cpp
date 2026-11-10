#include <iostream>
#include <fcntl.h>
#ifdef _WIN32
#include <io.h>
#endif

// Funkcja rysuje jeden piksel o podanym kolorze RGB na konkretnej pozycji X, Y
void drawPixelAt(int x, int y, int r, int g, int b) {
    // 1. Skaczemy kursorem na pozycję Y, X (uwaga: w ANSI podaje się Y, potem X, indeksowane od 1)
    std::wcout << L"\033[" << (y + 1) << L";" << (x * 2 + 1) << L"H";

    // 2. Ustawiamy kolor tła i wypisujemy dwie spacje (nasz kwadratowy piksel)
    std::wcout << L"\033[48;2;" << r << L";" << g << L";" << b << L"m  ";

    // 3. Resetujemy kolory, żeby nie zepsuć reszty konsoli
    std::wcout << L"\033[0m";
}

int main() {
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_U16TEXT); // Konfiguracja konsoli pod Visual Studio / Windows
#else
    std::locale::global(std::locale("C.UTF-8")); // lub std::locale("")
    std::wcout.imbue(std::locale("C.UTF-8"));
#endif
    std::wcout << L"\033[2J\033[H"; // Wyczyszczenie ekranu na starcie

    // Wypisujemy jakiś stały interfejs tekstowy, który ma stać w miejscu
    std::wcout << L"=== SILNIK GRAFICZNY W KONSOLI ===\n";
    std::wcout << L"Punkty: 1500\n";
    std::wcout << L"----------------------------------\n";

    // Rysujemy piksele w losowych miejscach, kompletnie ignorując tekst powyżej
    drawPixelAt(0, 0, 255, 0, 0);
    drawPixelAt(1, 1, 0, 255, 0);
    drawPixelAt(2, 2, 0, 0, 255);

    // Na koniec przesuwamy kursor bezpiecznie na dół, żeby tekst systemowy niczego nie zamazal
    std::wcout << L"\033[12;1H";

    std::wcout << L"\033[38;2;255;0;0m" << L"░░" << L" - 25%\n";
    std::wcout << L"\033[38;2;0;255;0m" << L"▒▒" << L" - 50%\n";
    std::wcout << L"\033[38;2;0;0;255m" << L"▓▓" << L" - 75%\n";
    std::wcout << L"\033[38;2;255;255;0m" << L"██" << L" - 100%\n\n";

    return 0;
}