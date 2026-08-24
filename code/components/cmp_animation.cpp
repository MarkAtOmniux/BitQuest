#include "cmp_animation.h"
#include <cmath>

using namespace std;
using namespace sf;

void AnimationComponent::Animation(std::string a, sf::Vector2f b, sf::IntRect animUvRect, sf::Vector2u imagecount)
{
	_texture = Resources::get<sf::Texture>(a);
	_sprite->setTexture(*_texture);
	_sprite->setTextureRect(animUvRect);
	_sprite->setOrigin(b);

	this->imagecount = imagecount;
	this->animUvRect = animUvRect;
	this->switchtime = 0.1f;

	attackImgNo = 0;
	currentimage.x = 0;
	totaltime = 0.0f;

	animUvRect.width = _texture->getSize().x / (imagecount.x);
	animUvRect.height = _texture->getSize().y / (imagecount.y);
}

AnimationComponent::AnimationComponent(Entity* p)
	: Component(p), _sprite(make_shared<sf::Sprite>())
{

}


void AnimationComponent::update(double dt) {

	_sprite->setPosition(_parent->getPosition());
	_sprite->setRotation(_parent->getRotation());

	if (flashTime > 0.f)
	{
		flashTime -= static_cast<float>(dt);
		const bool flashOn = (static_cast<int>(flashTime * 18.f) % 2) == 0;
		_sprite->setColor(flashOn ? Color(255, 255, 255) : Color(255, 70, 70));
		if (flashTime <= 0.f)
		{
			flashTime = 0.f;
			_sprite->setColor(Color::White);
		}
	}


	if (pause != true)
	{
		auto c = _parent->get_components<StateMachineComponent>()[0];


		if (c->currentState() == "Attack")
		{
			currentimage.y = 7;
			attackAnim(dt);

			if (attackImgNo == 7)
			{
				attackImgNo = 0;
			}
		}
		else if (c->currentState() == "chase" || c->currentState() == "walk_right" || c->currentState() == "walk_left" 
			|| c->currentState() == "flee")
		{

			currentimage.y = 0;
			Anim(dt);

		}
		else if (c->currentState() == "idle")
		{
			currentimage.y = 2;
			Anim(dt);
		}

		else if (c->currentState() == "idle")
		{
			currentimage.y = 7;
			attackAnim(dt);

			if (attackImgNo == 7)
			{
				attackImgNo = 0;
			}
		}
		else if (c->currentState() == "dead")
		{
			currentimage.y = 4;
			Anim(dt);
		}
		else if (c->currentState() == "cast")
		{
			currentimage.y = 6;
			attackAnim(dt);
		}
		else if(c->currentState() == "block")
		{
			currentimage.y = 3;
			Anim(dt);
		}
		else
		{
			Anim(dt);
		}
		if (attackImgNo != 0)
		{
			animDone = false;
		}
		if (attackImgNo == 0)
		{
			animDone = true;
		}
	}
}

void AnimationComponent::render() { Renderer::queue(_sprite.get()); }

void AnimationComponent::Anim(double dt)
{


	totaltime += dt;

	if (totaltime >= switchtime)
	{
		totaltime -= switchtime;
		currentimage.x++;


		if (currentimage.x >= imagecount.x)
		{
			currentimage.x = 0;
		}
	}


	animUvRect.top = currentimage.y *  animUvRect.height;


	if (faceRight)
	{
		animUvRect.left = currentimage.x * animUvRect.width;
		animUvRect.width = abs(animUvRect.width);
	}
	else
	{
		animUvRect.left = (currentimage.x + 1) * abs(animUvRect.width);
		animUvRect.width = -abs(animUvRect.width);
	}



	_sprite->setTextureRect(animUvRect);

}

void AnimationComponent::flashHit()
{
	flashTime = 0.28f;
}

void AnimationComponent::attackAnim(double dt)
{

	totaltime += dt;

	double frameTime = switchtime;
	if (_parent->getTags().count("player"))
	{
		frameTime = 0.05;
	}

	if (totaltime >= frameTime)
	{
		totaltime -= frameTime;
		attackImgNo++;

		if (attackImgNo >= imagecount.x)
		{
			attackImgNo = 0;
		}
	}


	animUvRect.top = currentimage.y *  animUvRect.height;

	if (faceRight)
	{
		animUvRect.left = attackImgNo * animUvRect.width;
		animUvRect.width = abs(animUvRect.width);
	}
	else
	{
		animUvRect.left = (attackImgNo + 1) * abs(animUvRect.width);
		animUvRect.width = -abs(animUvRect.width);
	}


	_sprite->setTextureRect(animUvRect);

}

void ShapeComponent::update(double dt) {

	_shape->setPosition(_parent->getPosition());
	_shape->setRotation(_parent->getRotation());


}


void ShapeComponent::render() { Renderer::queue(_shape.get()); }

sf::Shape& ShapeComponent::getShape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p)
	: Component(p), _shape(make_shared<sf::CircleShape>()) {}

sf::Sprite& AnimationComponent::getSprite() const { return *_sprite; }