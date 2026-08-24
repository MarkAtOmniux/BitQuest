#include "Prefabs.h"
#include "engine.h"
#include "GameState.h"
#include <levelsystem.h>
#include <system_resources.h>

#include "Player_states.h"
#include "goblin_states.h"
#include "orc_states.h"
#include "components/cmp_player_bullet.h"
#include "troll_states.h"
#include "skeleton_states.h"
#include "gavin_states.h"
#include "SaveLoad.h"

#include"ghost_states.h"
#include "components/cmp_ghost_properties.h"
#include "components/cmp_slime_bullet.h"
#include "components/cmp_slime_properties.h"
#include "Slime_states.h"
#include "components/cmp_goblin_properties.h"
#include "components/cmp_orc_properties.h"
#include "components/cmp_troll_properties.h"
#include "components/cmp_skeleton_properties.h"
#include "components/cmp_chest.h"

#include "components/cmp_gavin_properties.h"
#include "components/cmp_bullet.h"
#include "components/cmp_hurt.h"

#include"components/cmp_object_anim.h"
#include "components/cmp_state_Machine.h"
#include "components/cmp_btn.h"
#include "components/cmp_animation.h"
#include "components/cmp_player_controller.h"
#include "components/cmp_UI.h"
#include "components/cmp_shop.h"

using namespace std;
using namespace sf;

//**// Prefab Model \\**\\

/*
This model takes the current Entity->Component model and further enhances it, taking cues from the Unity 3D engine's prefab model. The
model take a number of components and turns them into an entity, this entity is then stored in a list, allowing the user to place the 
entities all across the game without duplicate code.

Our code does the exact same thing, using smart pointers we can create a list of a specific entities such as an enemy and place them
all over our level using the tile Engine to read them into the level, the order for this system goes as follows.


shared_ptr<Entity> makeSomething()
{
	auto something = Engine::GetActiveScene()->makeEntity();  create the entity
	something->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));  set the spawn location of the entity
	something->addTag("something");  give entity a tag for the state machine

	something->addComponent<SomeComponent>();  add component to the entity "something"
	something->addComponent<OtherComponent>();  add another component to the entity "something"

	return something;
}


We can then take this code and spawn it inside one of our levels using the line;

	makeSomething();
*/

shared_ptr<Entity> makePlayer(Vector2f _pos)
{
	auto player = Engine::GetActiveScene()->makeEntity();
	player->setPosition(_pos);
	player->addTag("player");
	
	auto sm = player->addComponent<StateMachineComponent>();

	sm->addState("idle", make_shared<Player_IdleState>());
	sm->addState("block", make_shared<Player_BlockState>());
	sm->addState("walk_left", make_shared<Player_MoveLeftState>());
	sm->addState("walk_right", make_shared<Player_MoveRightState>());
	sm->addState("Attack", make_shared<Player_AttackState>());
	sm->addState("dead", make_shared<Player_DeadState>());
	sm->changeState("idle");

	player->addComponent<PlayerControlerComponent>();

	player->addComponent<PlayerPhysicsComponent>(Vector2f(140, 160));
	auto a = player->addComponent<AnimationComponent>();
	a->Animation("spritesheets/Bob_spritesheet.png", Vector2f(0, 120), IntRect(0, 0, 240, 240), Vector2u(8, 8));
	a->getSprite().setOrigin(a->getSprite().getGlobalBounds().width / 2, a->getSprite().getGlobalBounds().height / 2);


	auto c = player->get_components<PlayerPhysicsComponent>()[0];

	b2Filter filter = c->getFixture()->GetFilterData();
	filter.categoryBits = 2;
	c->getFixture()->SetFilterData(filter);

	return player;
}


//------------------------ state machine test -----------------------------------------------------------
shared_ptr<Entity> makeGavin()
{
	auto gavin = Engine::GetActiveScene()->makeEntity();
	gavin->setPosition(ls::getTilePosition(ls::findTiles(ls::GAVIN)[0]));
	gavin->addTag("gavin");

	auto sm = gavin->addComponent<StateMachineComponent>();

	Engine::GetActiveScene()->ents.find("player")[0];

	sm->addState("dead", make_shared<Gavin_DeadState>());
	sm->addState("idle", make_shared<Gavin_IdleState>(Engine::GetActiveScene()->ents.find("player")[0]));
	sm->addState("chase", make_shared<Gavin_ChaseState>(Engine::GetActiveScene()->ents.find("player")[0]));
	sm->addState("cast", make_shared<Gavin_CastState>(Engine::GetActiveScene()->ents.find("player")[0]));

	auto bar = gavin->addComponent<SpriteComponent>();
	bar->Sprite("EnemyHealth.png", sf::IntRect(0, 0, 100, 5));
	bar->getSprite().setOrigin({ 50,100 });

	gavin->addComponent<GavinPropertiesComponent>();
	gavin->addComponent<GavinPhysicsComponent>(Vector2f(120, 160));
	auto a = gavin->addComponent<AnimationComponent>();
	a->Animation("spritesheets/Gavin_spritesheet.png", Vector2f(0, 120), IntRect(0, 0, 240, 240), Vector2u(8, 8));
	a->getSprite().setOrigin(a->getSprite().getGlobalBounds().width / 2, a->getSprite().getGlobalBounds().height / 2);

	sm->changeState("idle");



	auto c = gavin->get_components<GavinPhysicsComponent>()[0];

	b2Filter filter = c->getFixture()->GetFilterData();
	filter.categoryBits = 1;
	c->getFixture()->SetFilterData(filter);

	
	return gavin;
}
// ---------------------------------------------------------------------------------------------------------


vector<shared_ptr<Entity>> makeEnemies()
{
	vector<shared_ptr<Entity>> enemies;

	auto goblins = ls::findTiles(ls::ENEMY_GOBLIN);
	for (auto go : goblins)
	{
		auto goblin = Engine::GetActiveScene()->makeEntity();
		goblin->setPosition(ls::getTilePosition(go));
		goblin->addTag("goblin");
		
		auto p = goblin->addComponent<PhysicsComponent>(true, Vector2f(20, 80));
		auto sm = goblin->addComponent<StateMachineComponent>();
		p->getFixture()->GetBody()->SetBullet(true);
		
		auto gs = goblin->addComponent<GoblinPropertiesComponent>();

		sm->addState("dead", make_shared<Goblin_DeadState>());
		sm->addState("chase", make_shared<Goblin_ChaseState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->addState("idle", make_shared<Goblin_IdleState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->addState("Attack", make_shared<Goblin_AttackState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->changeState("idle");

		auto a = goblin->addComponent<AnimationComponent>();
		a->Animation("Spritesheets/Goblin_spritesheet.png", Vector2f(120, 240), IntRect(0, 0, 240, 240), Vector2u(8, 8));
		a->getSprite().setOrigin(a->getSprite().getGlobalBounds().width / 2, a->getSprite().getGlobalBounds().height / 2);

		auto bar = goblin->addComponent<SpriteComponent>();
		bar->Sprite("EnemyHealth.png", sf::IntRect(0, 0, 100, 5));
		bar->getSprite().setOrigin({ 50,100 });

		auto count = goblin->get_components<PhysicsComponent>();
		for (auto c : count)
		{
			b2Filter filter = c->getFixture()->GetFilterData();
			filter.categoryBits = 1;
			c->getFixture()->SetFilterData(filter);
		}

		enemies.push_back(goblin);
	}

	auto orcs = ls::findTiles(ls::ENEMY_ORC);
	for (auto orcTile : orcs)
	{
		auto orc = Engine::GetActiveScene()->makeEntity();
		orc->setPosition(ls::getTilePosition(orcTile));
		orc->addTag("orc");

		auto p = orc->addComponent<PhysicsComponent>(true, Vector2f(100, 160));
		auto sm = orc->addComponent<StateMachineComponent>();
		p->getFixture()->GetBody()->SetBullet(true);

		auto os = orc->addComponent<OrcPropertiesComponent>();

		sm->addState("dead", make_shared<Orc_DeadState>());
		sm->addState("chase", make_shared<Orc_ChaseState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->addState("idle", make_shared<Orc_IdleState>(Engine::GetActiveScene() ->ents.find("player")[0]));
		sm->addState("Attack", make_shared<Orc_AttackState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->changeState("idle");

		auto a = orc->addComponent<AnimationComponent>();
		a->Animation("Spritesheets/Orc_spritesheet.png", Vector2f(0, 120), IntRect(0, 0, 240, 240), Vector2u(8, 8));
		a->getSprite().setOrigin(a->getSprite().getLocalBounds().width / 2, a->getSprite().getGlobalBounds().height / 2);

		auto bar = orc->addComponent<SpriteComponent>();
		bar->Sprite("EnemyHealth.png", sf::IntRect(0, 0, 100, 5));
		bar->getSprite().setOrigin({ 50,100 });


		auto count = orc->get_components<PhysicsComponent>();
		for (auto c : count)
		{
			b2Filter filter = c->getFixture()->GetFilterData();
			filter.categoryBits = 1;
			c->getFixture()->SetFilterData(filter);
		}

		enemies.push_back(orc);
	}

	auto trolls = ls::findTiles(ls::ENEMY_TROLL);
	for (auto tr : trolls)
	{
		auto troll = Engine::GetActiveScene()->makeEntity();
		troll->setPosition(ls::getTilePosition(tr));
		troll->addTag("troll");
		auto prop = troll->addComponent<TrollPropertiesComponent>();

		auto sm = troll->addComponent<StateMachineComponent>();

		sm->addState("dead", make_shared<Troll_DeadState>());
		sm->addState("chase", make_shared<Troll_ChaseState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->addState("idle", make_shared<Troll_IdleState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->addState("Attack", make_shared<Troll_AttackState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->changeState("idle");


		troll->addComponent<PhysicsComponent>(true, Vector2f(160, 340));
		auto a = troll->addComponent<AnimationComponent>();
		a->Animation("Spritesheets/Troll_spritesheet.png", Vector2f(120, 240), IntRect(0, 0, 360, 360), Vector2u(8, 8));
		a->getSprite().setOrigin(a->getSprite().getGlobalBounds().width / 2, a->getSprite().getGlobalBounds().height / 2);

		auto bar = troll->addComponent<SpriteComponent>();
		bar->Sprite("EnemyHealth.png", sf::IntRect(0, 0, 100, 5));
		bar->getSprite().setOrigin({ 50,100 });


		auto count = troll->get_components<PhysicsComponent>();
		for (auto c : count)
		{
			b2Filter filter = c->getFixture()->GetFilterData();
			filter.categoryBits = 1;
			c->getFixture()->SetFilterData(filter);
		}

		enemies.push_back(troll);
	}

	auto skeletons = ls::findTiles(ls::ENEMY_SKELETON);
	for (auto sk : skeletons)
	{
		auto skeleton = Engine::GetActiveScene()->makeEntity();
		skeleton->setPosition(ls::getTilePosition(sk));
		skeleton->addTag("skeleton");
		auto prop = skeleton->addComponent<SkeletonPropertiesComponent>();


		auto sm = skeleton->addComponent<StateMachineComponent>();

		sm->addState("dead", make_shared<Skeleton_DeadState>());
		sm->addState("chase", make_shared<Skeleton_ChaseState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->addState("idle", make_shared<Skeleton_IdleState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->addState("Attack", make_shared<Skeleton_AttackState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->changeState("idle");


		skeleton->addComponent<PhysicsComponent>(true, Vector2f(100, 160));
		auto a = skeleton->addComponent<AnimationComponent>();
		a->Animation("Spritesheets/skeleton_spritesheet.png", Vector2f(0, 120), IntRect(0, 0, 240, 240), Vector2u(8, 8));
		a->getSprite().setOrigin(a->getSprite().getGlobalBounds().width / 2, a->getSprite().getGlobalBounds().height / 2);

		auto bar = skeleton->addComponent<SpriteComponent>();
		bar->Sprite("EnemyHealth.png", sf::IntRect(0, 0, 100, 5));
		bar->getSprite().setOrigin({ 50,100 });

		auto count = skeleton->get_components<PhysicsComponent>();
		for (auto c : count)
		{
			b2Filter filter = c->getFixture()->GetFilterData();
			filter.categoryBits = 1;
			c->getFixture()->SetFilterData(filter);
		}



		enemies.push_back(skeleton);
	}

	auto slimes = ls::findTiles(ls::ENEMY_SLIME);
	for (auto sl : slimes)
	{
		auto slime = Engine::GetActiveScene()->makeEntity();
		slime->setPosition(ls::getTilePosition(sl));
		slime->addTag("slime");


		slime->addComponent<SlimePropertiesComponent>();

		auto sm = slime->addComponent<StateMachineComponent>();

		sm->addState("dead", make_shared<Slime_DeadState>());
		sm->addState("flee", make_shared<Slime_FleeState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->addState("idle", make_shared<Slime_IdleState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->addState("Attack", make_shared<Slime_AttackState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->changeState("idle");


		slime->addComponent<PhysicsComponent>(true, Vector2f(80, 200));
		auto a = slime->addComponent<AnimationComponent>();
		a->Animation("Spritesheets/Slime_spritesheet.png", Vector2f(120, 240), IntRect(0, 0, 240, 240), Vector2u(8, 8));
		a->getSprite().setOrigin(a->getSprite().getGlobalBounds().width / 2, a->getSprite().getGlobalBounds().height / 2);

		auto bar = slime->addComponent<SpriteComponent>();
		bar->Sprite("EnemyHealth.png", sf::IntRect(0, 0, 100, 5));
		bar->getSprite().setOrigin({ 50,100 });

		enemies.push_back(slime);
	}

	auto ghosts = ls::findTiles(ls::ENEMY_GHOST);
	for (auto gh : ghosts)
	{
		auto ghost = Engine::GetActiveScene()->makeEntity();
		ghost->setPosition(ls::getTilePosition(gh));
		ghost->addTag("ghost");

		ghost->addComponent<GhostPropertiesComponent>();

		auto sm = ghost->addComponent<StateMachineComponent>();

		sm->addState("dead", make_shared<Ghost_DeadState>());
		sm->addState("idle", make_shared<Ghost_IdleState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->addState("chase", make_shared<Ghost_ChaseState>(Engine::GetActiveScene()->ents.find("player")[0]));
		sm->addState("cast", make_shared<Ghost_CastState>(Engine::GetActiveScene()->ents.find("player")[0]));

		ghost->addComponent<PhysicsComponent>(true, Vector2f(100, 200));
		auto a = ghost->addComponent<AnimationComponent>();
		a->Animation("Spritesheets/Ghost_spritesheet.png", Vector2f(120, 240), IntRect(0, 0, 240, 240), Vector2u(8, 8));
		a->getSprite().setOrigin(a->getSprite().getGlobalBounds().width / 2, a->getSprite().getGlobalBounds().height / 2);
		
		auto count = ghost->get_components<PhysicsComponent>();
		for (auto c : count)
		{
			b2Filter filter = c->getFixture()->GetFilterData();
			filter.categoryBits = 1;
			c->getFixture()->SetFilterData(filter);
		}

		auto bar = ghost->addComponent<SpriteComponent>();
		bar->Sprite("EnemyHealth.png", sf::IntRect(0, 0, 100, 5));
		bar->getSprite().setOrigin({ 50,100 });

		sm->changeState("idle");
		enemies.push_back(ghost);
	}
	return enemies;
}

vector<shared_ptr<Entity>> makeShops()
{
	vector<shared_ptr<Entity>> Mshops;

	auto shops = ls::findTiles(ls::SHOP);
	for (auto s : shops)
	{
		auto shop = Engine::GetActiveScene()->makeEntity();
		shop->setPosition(ls::getTilePosition(s));
		shop->addTag("Shop");

		auto a = shop->addComponent <ObjectAnimComponent>();
		a->Animation("spritesheets/Shop_spritesheet.png", Vector2f(0,0), IntRect(0, 0, 240, 240), Vector2u(8, 1));
		a->getSprite().setOrigin(0, 0);

		auto t = shop->addComponent<TextComponent>("Press 'E' to enter shop!");
		t->getText().setOrigin(150, 50);

		shop->addComponent<ShopComponent>();
		Mshops.push_back(shop);
	}
	return Mshops;

}

vector<shared_ptr<Entity>> makeTorches()
{
	vector<shared_ptr<Entity>> Mtorches;

	auto torches = ls::findTiles(ls::TORCH);
	for (auto t : torches)
	{
		auto torch = Engine::GetActiveScene()->makeEntity();
		torch->setPosition(ls::getTilePosition(t));
		torch->addTag("torch");

		auto a = torch->addComponent<ObjectAnimComponent>();
		a->Animation("spritesheets/Torch.png", Vector2f(0, 0), IntRect(0, 0, 120, 120), Vector2u(8, 1));
		a->getSprite().setOrigin(0, 0);

		Mtorches.push_back(torch);
	}

	return Mtorches;
}

vector<shared_ptr<Entity>> makeChests()
{
	vector<shared_ptr<Entity>> Mchests;

	auto chests = ls::findTiles(ls::CHEST);
	for (auto c : chests)
	{
		auto chest = Engine::GetActiveScene()->makeEntity();
		chest->setPosition(ls::getTilePosition(c));
		chest->addTag("chest");

		auto t = chest->addComponent<TextComponent>("press E to open chest");
		t->getText().setOrigin(160, 60);

		auto s = chest->addComponent <SpriteComponent>();
		s->Sprite("spritesheets/Chest.png", IntRect(0, 0, 120, 120));
		s->getSprite().setOrigin(0, -140);

		auto a = chest->addComponent<ChestComponent>();

		Mchests.push_back(chest);
	}
	return Mchests;

}


shared_ptr<Entity> makeEventBox(Vector2f bounds)
{
	auto eventBox = Engine::GetActiveScene()->makeEntity();
	eventBox->addTag("eventBox");

	auto s = eventBox->addComponent<ShapeComponent>();
	s->setShape<RectangleShape>(Vector2f(bounds));
	s->getShape().setFillColor(Color::Red);
	s->getShape().setOrigin(s->getShape().getGlobalBounds().width / 2, s->getShape().getGlobalBounds().height / 2);

	return eventBox;

}


shared_ptr<Entity> makeCoin(Vector2f pos)
{

		auto coin = Engine::GetActiveScene()->makeEntity();
		coin->addTag("coin");

		coin->setPosition(pos);

		coin->addComponent<PhysicsComponent>(true, Vector2f(30, 60));
		auto a = coin->addComponent<ObjectAnimComponent>();
		a->Animation("spritesheets/Coin_spritesheet.png", Vector2f(0, -60), IntRect(0, 0, 60, 60), Vector2u(6, 1));
		a->getSprite().setOrigin(a->getSprite().getGlobalBounds().width / 2, a->getSprite().getGlobalBounds().height / 2);

		return coin;

}
	


shared_ptr<Entity> makeButton(string text, Vector2f bounds)
{

	auto button = Engine::GetActiveScene()->makeEntity();
	button->addTag("button");

	auto s = button->addComponent<ShapeComponent>();
	s->setShape<RectangleShape>(Vector2f(bounds));
	s->getShape().setFillColor(Color::Transparent);
	s->getShape().setOrigin(s->getShape().getGlobalBounds().width / 2, s->getShape().getGlobalBounds().height / 2);

	auto u = button->addComponent<ShapeComponent>();
	u->setShape<RectangleShape>(Vector2f(bounds.x, 5.f));
	u->getShape().setFillColor(Color(128, 128, 128));
	u->getShape().setOrigin(u->getShape().getGlobalBounds().width / 2, u->getShape().getGlobalBounds().height / 2 - 30);

	auto t = button->addComponent<TextComponent>(text);
	t->getText().setOrigin(t->getText().getGlobalBounds().width / 2, t->getText().getGlobalBounds().height / 2);

	button->addComponent<BtnComponent>(s, t, u);

	return button;
}

void TilePhysics()
{
	auto walls = ls::findTiles(ls::WALL);
	for (auto w : walls) {
		auto pos = ls::getTilePosition(w);
		pos += Vector2f(ls::getTileSize() / 2, ls::getTileSize() / 2);
		auto e = Engine::GetActiveScene()->makeEntity();
		e->setPosition(pos);
		e->addComponent<PhysicsComponent>(false, Vector2f(ls::getTileSize(), ls::getTileSize()));
		auto count = e->get_components<PhysicsComponent>();
		for (auto c : count)
		{
			b2Filter filter = c->getFixture()->GetFilterData();
			filter.categoryBits = 2;
			c->getFixture()->SetFilterData(filter);
		}
		e->addTag("wall");
	}

	auto floor = ls::findTiles(ls::FLOOR);
	for (auto f : floor) {
		auto pos = ls::getTilePosition(f);
		pos += Vector2f(ls::getTileSize() / 2, ls::getTileSize() / 2);
		auto e = Engine::GetActiveScene()->makeEntity();
		e->setPosition(pos);
		e->addComponent<PhysicsComponent>(false, Vector2f(ls::getTileSize(), ls::getTileSize()));
		auto count = e->get_components<PhysicsComponent>();
		for (auto c : count)
		{
			b2Filter filter = c->getFixture()->GetFilterData();
			filter.categoryBits = 2;
			c->getFixture()->SetFilterData(filter);
		}
		e->addTag("floor");
		
	}
}

shared_ptr<Entity>addUI(string message)
{
	auto e = Engine::GetActiveScene()->makeEntity();
	e->addTag("UI");


	auto c = e->addComponent<TextComponent>(to_string(SaveLoad::coins));
	c->getText().setOrigin(c->getText().getGlobalBounds().width / 2, c->getText().getGlobalBounds().height / 2);

	auto a = e->addComponent<TextComponent>(to_string(SaveLoad::arrows));
	a->getText().setOrigin(a->getText().getGlobalBounds().width / 2, a->getText().getGlobalBounds().height / 2);

	auto h = e->addComponent<TextComponent>(to_string(SaveLoad::hams));
	h->getText().setOrigin(h->getText().getGlobalBounds().width / 2, h->getText().getGlobalBounds().height / 2);


	auto cutsceneBar = e->addComponent<ShapeComponent>();
	cutsceneBar->setShape<RectangleShape>(Vector2f(game_width, 150));
	cutsceneBar->getShape().setFillColor(Color::Transparent);
	cutsceneBar->getShape().setOrigin(0, 0);


	auto cutsceneText = e->addComponent<TextComponent>(message);
	cutsceneText->getText().setOrigin(cutsceneText->getText().getGlobalBounds().width / 2, cutsceneText->getText().getGlobalBounds().height / 2);
	
	e->addComponent<UIComponent>(c,a,h, cutsceneText, cutsceneBar);

	return e;
}


shared_ptr<Entity>GavinBlast()
{
	auto gb = Engine::GetActiveScene()->makeEntity();
	auto a = gb->addComponent<SpriteComponent>();

	if (gb->scene->ents.find("player")[0]->getPosition().x < gb->scene->ents.find("gavin")[0]->getPosition().x)
	{
		a->Sprite("magic.png", IntRect(40, 0, -40, 40));
		gb->setPosition(Vector2f(gb->scene->ents.find("gavin")[0]->getPosition().x - 50 , gb->scene->ents.find("gavin")[0]->getPosition().y - 20));
		gb->setRotation(180.f);
	}
	else
	{
		gb->setPosition(Vector2f(gb->scene->ents.find("gavin")[0]->getPosition().x + 10, gb->scene->ents.find("gavin")[0]->getPosition().y - 20));
		a->Sprite("magic.png", IntRect(0, 0, 40, 40));
	}
	
	gb->addComponent<PhysicsComponent>(true, Vector2f(10, 40));
	gb->addComponent<BulletComponent>();
	
	auto p = gb->get_components<PhysicsComponent>()[0];
	b2Filter filter = p->getFixture()->GetFilterData();
	filter.maskBits = 2;
	p->getFixture()->SetFilterData(filter);

	gb->addTag("gavBlast");

	return gb;
}

shared_ptr<Entity>makeArrow()
{
	auto pa = Engine::GetActiveScene()->makeEntity();
	auto a = pa->addComponent<SpriteComponent>();

	a->Sprite("Arrow.png", IntRect(80, 0, -80, 40));
	if (pa->scene->ents.find("player")[0]->get_components<AnimationComponent>()[0]->faceRight == true)
	{
		pa->setPosition(Vector2f(pa->scene->ents.find("player")[0]->getPosition().x + 50, pa->scene->ents.find("player")[0]->getPosition().y - 20));
		a->getSprite().setOrigin(a->getSprite().getGlobalBounds().width / 2, a->getSprite().getGlobalBounds().height / 2);
	}
	else
	{
		pa->setPosition(Vector2f(pa->scene->ents.find("player")[0]->getPosition().x - 10, pa->scene->ents.find("player")[0]->getPosition().y - 20));
		a->Sprite("Arrow.png", IntRect(0, 0, 80, 40));
		a->getSprite().setOrigin(a->getSprite().getGlobalBounds().width / 2, a->getSprite().getGlobalBounds().height / 2);
		pa->setRotation(180.f);
	}

	pa->addComponent<PhysicsComponent>(true, Vector2f(10, 40));
	pa->addComponent<PlayerBulletComponent>();

	pa->addTag("playerArrow");

	return pa;
}

shared_ptr<Entity>slimeBlast()
{
	auto gb = Engine::GetActiveScene()->makeEntity();
	auto a = gb->addComponent<SpriteComponent>();

	if (gb->scene->ents.find("player")[0]->getPosition().x < gb->scene->ents.find("slime")[0]->getPosition().x)
	{
		a->Sprite("SlimeBall.png", IntRect(80, 0, -80, 80));
		gb->setPosition(Vector2f(gb->scene->ents.find("slime")[0]->getPosition().x - 50, gb->scene->ents.find("slime")[0]->getPosition().y + 40));
		gb->setRotation(180.f);
	}
	else
	{
		gb->setPosition(Vector2f(gb->scene->ents.find("slime")[0]->getPosition().x + 50, gb->scene->ents.find("slime")[0]->getPosition().y + 40));
		a->Sprite("SlimeBall.png", IntRect(0, 0, 80, 80));
	}

	gb->addComponent<PhysicsComponent>(true, Vector2f(80, 80));
	gb->addComponent<SlimeBulletComponent>();
	auto p = gb->get_components<PhysicsComponent>()[0];

	b2Filter filter = p->getFixture()->GetFilterData();
	filter.maskBits = 2;
	p->getFixture()->SetFilterData(filter);

	gb->addTag("slimeBlast");

	return gb;
}

shared_ptr<Entity>ghostBlast()
{
	auto gb = Engine::GetActiveScene()->makeEntity();
	auto a = gb->addComponent<SpriteComponent>();

	if (gb->scene->ents.find("player")[0]->getPosition().x < gb->scene->ents.find("ghost")[0]->getPosition().x)
	{
		a->Sprite("Ghostmagic.png", IntRect(40, 0, -40, 40));
		gb->setPosition(Vector2f(gb->scene->ents.find("ghost")[0]->getPosition().x - 50, gb->scene->ents.find("ghost")[0]->getPosition().y - 40));
		gb->setRotation(180.f);
	}
	else
	{
		gb->setPosition(Vector2f(gb->scene->ents.find("ghost")[0]->getPosition().x + 50, gb->scene->ents.find("ghost")[0]->getPosition().y - 40));
		a->Sprite("Ghostmagic.png", IntRect(0, 0, 40, 40));
	}

	gb->addComponent<PhysicsComponent>(true, Vector2f(10, 40));
	gb->addComponent<BulletComponent>();
	auto p = gb->get_components<PhysicsComponent>()[0];

	b2Filter filter = p->getFixture()->GetFilterData();
	filter.maskBits = 2;
	p->getFixture()->SetFilterData(filter);

	gb->addTag("ghostBlast");

	return gb;
}