#include "Controls.h"
#include "GameState.h"
#include <stdexcept>

using namespace std;
using namespace sf;

map<std::string, Keyboard::Key> Controls::_keyboardLookupTable;

void Controls::initialise()
{
		Controls::SetKeyboardButton("Action", Keyboard::E);
		Controls::SetKeyboardButton("MoveLeft", Keyboard::A);
		Controls::SetKeyboardButton("MoveRight", Keyboard::D);
		Controls::SetKeyboardButton("Jump", Keyboard::W);
		Controls::SetKeyboardButton("Attack", Keyboard::Space);
		Controls::SetKeyboardButton("Block", Keyboard::C);
		Controls::SetKeyboardButton("Sword", Keyboard::Num1);
		Controls::SetKeyboardButton("Bow", Keyboard::Num2);
		Controls::SetKeyboardButton("Ham", Keyboard::H);
}

Keyboard::Key Controls::GetKeyboardButton(string action)
{
	auto k = _keyboardLookupTable.find(action);
	if (k != _keyboardLookupTable.end())
	{
		return k->second;
	}
	else
	{
		throw invalid_argument("Action not known");
	}
}

void Controls::SetKeyboardButton(string action, Keyboard::Key key)
{
	_keyboardLookupTable[action] = key;
}

bool Controls::isPressed(const string& action)
{
	// Space is always attack, never jump — ignore any remap that would swap them.
	if (action == "Attack")
	{
		return Keyboard::isKeyPressed(Keyboard::Space);
	}
	if (action == "Jump")
	{
		return Keyboard::isKeyPressed(Keyboard::W)
			|| Keyboard::isKeyPressed(Keyboard::Up);
	}
	if (action == "Block")
	{
		return Keyboard::isKeyPressed(Keyboard::C);
	}

	if (Keyboard::isKeyPressed(GetKeyboardButton(action)))
	{
		return true;
	}

	if (action == "MoveLeft" && Keyboard::isKeyPressed(Keyboard::Left))
	{
		return true;
	}
	if (action == "MoveRight" && Keyboard::isKeyPressed(Keyboard::Right))
	{
		return true;
	}

	return false;
}
