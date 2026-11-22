#include "Stats.h"
#include <format>

// --- Stat ---

Stat::Stat() : display_(nullptr), row_(0), label_(L"")
{
}

Stat::Stat(Display& display, int row, const std::wstring& label) : display_(&display), row_(row), label_(label)
{
}

void Stat::Redraw()
{
	if (display_ == nullptr)
		return; // obiekt z konstruktora domyślnego - nie ma gdzie rysować

	std::wstring text = label_.empty() ? ValueText() : label_ + L": " + ValueText();
	if (text == lastDrawn_)
		return; // na ekranie już jest ten sam tekst

	display_->DrawText(row_, text);
	lastDrawn_ = text;
}

// --- IntStat ---

IntStat::IntStat() : Stat(), value_(0)
{
}

IntStat::IntStat(Display& display, int row, const std::wstring& label, int value)
	: Stat(display, row, label), value_(value)
{
	Redraw(); // pierwsze narysowanie od razu po utworzeniu
}

void IntStat::Set(int value) { value_ = value; Redraw(); }
void IntStat::Add(int delta) { value_ += delta; Redraw(); }
int IntStat::Get() const { return value_; }

// Operatory zwracają *this, żeby działało też np. a = b = 5;
IntStat& IntStat::operator=(int value) { Set(value); return *this; }
IntStat& IntStat::operator+=(int delta) { Add(delta); return *this; }

std::wstring IntStat::ValueText() const
{
	return std::to_wstring(value_);
}

// --- FloatStat ---

FloatStat::FloatStat() : Stat(), value_(0.0)
{
}

FloatStat::FloatStat(Display& display, int row, const std::wstring& label, double value)
	: Stat(display, row, label), value_(value)
{
	Redraw();
}

void FloatStat::Set(double value) { value_ = value; Redraw(); }
double FloatStat::Get() const { return value_; }

FloatStat& FloatStat::operator=(double value) { Set(value); return *this; }
FloatStat& FloatStat::operator+=(double delta) { Set(value_ + delta); return *this; }

std::wstring FloatStat::ValueText() const
{
	return std::format(L"{:.2f}", value_); // 2 miejsca po przecinku
}

// --- TextStat ---

TextStat::TextStat() : Stat(), text_(L"")
{
}

TextStat::TextStat(Display& display, int row, const std::wstring& text)
	: Stat(display, row, L""), text_(text)
{
	Redraw();
}

void TextStat::Set(const std::wstring& text) { text_ = text; Redraw(); }

TextStat& TextStat::operator=(const std::wstring& text) { Set(text); return *this; }

std::wstring TextStat::ValueText() const
{
	return text_;
}
