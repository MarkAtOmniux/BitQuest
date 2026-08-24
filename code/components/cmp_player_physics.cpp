#include "cmp_player_physics.h"
#include "../code/Controls.h"
#include <cmath>


using namespace std;
using namespace sf;
using namespace Physics;

namespace {
	// Old jump was clamped to 800 px/s under 1x gravity. Scale takeoff by
	// sqrt(gravity) so peak height stays the same while air time drops.
	constexpr float kJumpGravityScale = 2.f;
	constexpr float kOldJumpSpeed = 800.f;
}

bool PlayerPhysicsComponent::isGrounded() const {
	auto touch = getTouching();
	const auto& pos = _body->GetPosition();
	const float halfPlrHeigt = _size.y * .5f;
	const float halfPlrWidth = _size.x * .52f;
	b2WorldManifold manifold;
	for (const auto& contact : touch) {
		contact->GetWorldManifold(&manifold);
		const int numPoints = contact->GetManifold()->pointCount;
		bool onTop = numPoints > 0;
		// If all contacts are below the player.
		for (int j = 0; j < numPoints; j++) {
			onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
		}
		if (onTop) {
			return true;
		}
	}

	return false;
}


void PlayerPhysicsComponent::update(double dt) {

	const auto pos = _parent->getPosition();

	if (pos.y > ls::getHeight() * ls::getTileSize())
	{
		teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	}



	// Jump is W / Up only. Space is attack and must never apply this impulse.
	if (Controls::isPressed("Jump") && !Keyboard::isKeyPressed(Keyboard::Space))
	{
		_grounded = isGrounded();
		if (_grounded) {
			const float jumpSpeed = kOldJumpSpeed * std::sqrt(kJumpGravityScale);
			setVelocity(Vector2f(getVelocity().x, jumpSpeed));
			teleport(Vector2f(pos.x, pos.y - 5.0f));
		}
	}

	if (getVelocity().y < 0.f)
	{
		_body->SetGravityScale(kJumpGravityScale * 1.35f);
	}
	else
	{
		_body->SetGravityScale(kJumpGravityScale);
	}

	//Are we in air?
	if (!_grounded)
	{
		// Check to see if we have landed yet
		_grounded = isGrounded();
		// disable friction while jumping
		setFriction(0.f);
	}
	else
	{
		setFriction(0.1f);
	}

	// Clamp velocity.
	auto v = getVelocity();
	v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
	v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
	setVelocity(v);


	PhysicsComponent::update(dt);
}


PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p,
	const Vector2f& size)
	: PhysicsComponent(p, true, size) {
	_size = sv2_to_bv2(size, true);
	_maxVelocity = Vector2f(400.f, kOldJumpSpeed * std::sqrt(kJumpGravityScale) + 1.f);
	_groundspeed = 200.f;
	_grounded = false;
	_body->SetSleepingAllowed(false);
	_body->SetFixedRotation(true);
	_body->SetBullet(true);
	_body->SetGravityScale(kJumpGravityScale);
	setRestitution(0.f);

}

void PlayerPhysicsComponent::MoveLeft(double dt)
{

	if (getVelocity().x > -_maxVelocity.x)
		impulse({ -(float)(dt * _groundspeed), 0 });
}

void PlayerPhysicsComponent::MoveRight(double dt)
{
	if (getVelocity().x < _maxVelocity.x)
		impulse({ (float)(dt * _groundspeed), 0 });
}