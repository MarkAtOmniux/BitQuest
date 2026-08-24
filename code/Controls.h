#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class Controls
{
private:
	static std::map<std::string, sf::Keyboard::Key> _keyboardLookupTable;

public:
	static void initialise();
	static sf::Keyboard::Key GetKeyboardButton(std::string action);
	static void SetKeyboardButton(std::string action, sf::Keyboard::Key key);
};