#include "cmp_chest.h"
#include "engine.h"
#include "../code/SaveLoad.h"
#include "../code/Controls.h"
#include "../components/cmp_sprite.h"

using namespace std;
using namespace sf;

ChestComponent::ChestComponent(Entity* p)
	: Component(p)
{

	_player = _parent->scene->ents.find("player")[0];

}

void ChestComponent::update(double dt)
{
	if (length(_parent->getPosition() - _player->getPosition()) < 200.f)
	{
		if (Engine::isKeyDown(Controls::GetKeyboardButton("Action")))
		{
			SaveLoad::coins = SaveLoad::coins + 50;

			_parent->setForDelete();
		}
	}
}

