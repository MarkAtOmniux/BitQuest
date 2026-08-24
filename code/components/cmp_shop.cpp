#include "cmp_shop.h"
#include "engine.h"
#include <SFML/Window/Mouse.hpp>
#include "../code/Controls.h"
#include "../GameState.h"
#include "../code/SaveLoad.h"

using namespace std;
using namespace sf;

ShopComponent::ShopComponent(Entity* p)
: Component(p)
{
	_player = _parent->scene->ents.find("player")[0];
}

void ShopComponent::update(double dt)
{
	if (length(_parent->getPosition() - _player->getPosition()) < 300.f)
	{
		if (Engine::isKeyDown(Controls::GetKeyboardButton("Action")))
		{

			Vector2f currentPos = _player->getPosition();

			int posX = currentPos.x;
			int posY = currentPos.y;

			Vector2u saveCoords = Vector2u(((posX + 240 / 2) / 240) * 240, ((posY + 240) / 240) * 240);

			SaveLoad::positionX = saveCoords.x;
			SaveLoad::positionY = saveCoords.y - 240;

			SaveLoad::SaveGame();

			sleep(milliseconds(50));

			Engine::ChangeScene(&shop);
		}
	}
}


