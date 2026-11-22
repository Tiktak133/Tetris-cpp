#pragma once
#include <string>
#include "Display.h"

// Klasa bazowa: jeden wiersz statystyk z prawej strony planszy.
// Przerysowuje się sama przy każdej zmianie wartości - nie trzeba nic odświeżać ręcznie.
class Stat
{
public:
	Stat();
	Stat(Display& display, int row, const std::wstring& label);
	virtual ~Stat() = default;

	std::wstring Text() const;   // "etykieta: wartość" - wartość daje wirtualne ValueText() klasy pochodnej

protected:
	virtual std::wstring ValueText() const = 0;
	void Redraw();

private:
	Display* display_;
	int row_;
	std::wstring label_;
	std::wstring lastDrawn_;
};

// Liczba całkowita
class IntStat : public Stat
{
public:
	IntStat();
	IntStat(Display& display, int row, const std::wstring& label, int value = 0);

	void Set(int value);
	void Add(int delta);
	int Get() const;

	IntStat& operator=(int value);   // score = 100;
	IntStat& operator+=(int delta);  // score += 100;

protected:
	std::wstring ValueText() const override;

private:
	int value_;
};

// Liczba z przecinkiem
class FloatStat : public Stat
{
public:
	FloatStat();
	FloatStat(Display& display, int row, const std::wstring& label, double value = 0.0);

	void Set(double value);
	double Get() const;

	FloatStat& operator=(double value);   // time = 12.5;
	FloatStat& operator+=(double delta);  // time += dt;

protected:
	std::wstring ValueText() const override;

private:
	double value_;
};

// Sam tekst
class TextStat : public Stat
{
public:
	TextStat();
	TextStat(Display& display, int row, const std::wstring& text);

	void Set(const std::wstring& text);

	TextStat& operator=(const std::wstring& text);  // title = L"PAUZA";

protected:
	std::wstring ValueText() const override;

private:
	std::wstring text_;
};
