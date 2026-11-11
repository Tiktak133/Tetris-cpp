#include "Actions.h"
#include <conio.h>

Action ReadAction()
{
	int c = _getch();
	if (c == 0 || c == 224) // strzałki: prefiks + drugi kod
	{
		switch (_getch())
		{
			case 75: return Action::Left;
			case 77: return Action::Right;
			case 80: return Action::Down;
			case 72: return Action::Rotate;
		}
		return Action::None;
	}
	switch (c) // litery: małe i wielkie (włączony Caps Lock)
	{
		case 'a': case 'A': return Action::Left;
		case 'd': case 'D': return Action::Right;
		case 's': case 'S': return Action::Down;
		case 'w': case 'W': return Action::Rotate;
		case 27:            return Action::Quit; // Esc
	}
	return Action::None;
}