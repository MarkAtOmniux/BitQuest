#include "Controls.h"
#include "GameState.h"
#include "engine.h"
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
		return Engine::isKeyDown(Keyboard::Space);
	}
	if (action == "Jump")
	{
		return Engine::isKeyDown(Keyboard::W)
			|| Engine::isKeyDown(Keyboard::Up);
	}
	if (action == "Block")
	{
		return Engine::isKeyDown(Keyboard::C);
	}

	if (Engine::isKeyDown(GetKeyboardButton(action)))
	{
		return true;
	}

	if (action == "MoveLeft" && Engine::isKeyDown(Keyboard::Left))
	{
		return true;
	}
	if (action == "MoveRight" && Engine::isKeyDown(Keyboard::Right))
	{
		return true;
	}

	return false;
}
