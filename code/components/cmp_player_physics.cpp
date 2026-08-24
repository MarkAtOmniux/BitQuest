#include "cmp_player_physics.h"
#include "../code/Controls.h"
#include <cmath>
#include <string>
#include <fstream>
#include <chrono>


using namespace std;
using namespace sf;
using namespace Physics;

namespace {
	// Old jump was clamped to 800 px/s under 1x gravity. Scale takeoff by
	// sqrt(gravity) so peak height stays the same while air time drops.
	constexpr float kJumpGravityScale = 2.f;
	constexpr float kOldJumpSpeed = 800.f;

	// #region agent log
	void dbgLog(const char* hid, const char* loc, const char* msg, const string& dataJson)
	{
		ofstream f("/Users/markbarton/Documents/BitQuest/.cursor/debug-6bcd73.log", ios::app);
		if (!f) return;
		const auto ts = chrono::duration_cast<chrono::milliseconds>(
			chrono::system_clock::now().time_since_epoch()).count();
		f << "{\"sessionId\":\"6bcd73\",\"hypothesisId\":\"" << hid
			<< "\",\"location\":\"" << loc << "\",\"message\":\"" << msg
			<< "\",\"data\":" << dataJson << ",\"timestamp\":" << ts << "}\n";
	}
	// #endregion
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
		// #region agent log
		dbgLog("H4", "cmp_player_physics.cpp:update", "teleport to start",
			string("{\"py\":") + to_string(pos.y) + ",\"limit\":" + to_string(ls::getHeight() * ls::getTileSize()) + "}");
		// #endregion
		teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	}

	_grounded = isGrounded();

	std::string state;
	auto sms = _parent->get_components<StateMachineComponent>();
	if (!sms.empty())
	{
		state = sms[0]->currentState();
	}
	const bool canSteer = (state != "dead" && state != "block");

	if (canSteer)
	{
		if (Controls::isPressed("MoveLeft") && !Controls::isPressed("MoveRight"))
		{
			impulse({ -15.0f, 0.0f });
		}
		if (Controls::isPressed("MoveRight") && !Controls::isPressed("MoveLeft"))
		{
			impulse({ 15.0f, 0.0f });
		}
	}

	if (canSteer && Controls::isPressed("Jump") && !Engine::isKeyDown(Keyboard::Space))
	{
		if (_grounded) {
			const float jumpSpeed = kOldJumpSpeed * std::sqrt(kJumpGravityScale);
			setVelocity(Vector2f(getVelocity().x, jumpSpeed));
			teleport(Vector2f(pos.x, pos.y - 5.0f));
			_grounded = false;
		}
	}

	if (_grounded)
	{
		_body->SetGravityScale(1.f);
		setFriction(0.1f);
	}
	else
	{
		setFriction(0.f);
		if (getVelocity().y < 0.f)
		{
			_body->SetGravityScale(kJumpGravityScale * 1.35f);
		}
		else
		{
			_body->SetGravityScale(kJumpGravityScale);
		}
	}

	auto v = getVelocity();
	v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
	v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
	setVelocity(v);

	// #region agent log
	{
		static int dbgN = 0;
		++dbgN;
		const bool left = Controls::isPressed("MoveLeft");
		const bool right = Controls::isPressed("MoveRight");
		const bool rawA = Keyboard::isKeyPressed(Keyboard::A);
		const bool rawD = Keyboard::isKeyPressed(Keyboard::D);
		const bool rawLeft = Keyboard::isKeyPressed(Keyboard::Left);
		const bool rawRight = Keyboard::isKeyPressed(Keyboard::Right);
		const bool evtA = Engine::isKeyDown(Keyboard::A);
		const bool evtD = Engine::isKeyDown(Keyboard::D);
		const bool evtLeft = Engine::isKeyDown(Keyboard::Left);
		const bool evtRight = Engine::isKeyDown(Keyboard::Right);
		if (dbgN <= 5 || (dbgN % 45 == 0) || left || right || rawA || rawD || rawLeft || rawRight || evtA || evtD || evtLeft || evtRight)
		{
			dbgLog("H1", "cmp_player_physics.cpp:update", "player physics tick",
				string("{\"n\":") + to_string(dbgN)
				+ ",\"left\":" + (left ? "true" : "false")
				+ ",\"right\":" + (right ? "true" : "false")
				+ ",\"rawA\":" + (rawA ? "true" : "false")
				+ ",\"rawD\":" + (rawD ? "true" : "false")
				+ ",\"rawLeft\":" + (rawLeft ? "true" : "false")
				+ ",\"rawRight\":" + (rawRight ? "true" : "false")
				+ ",\"evtA\":" + (evtA ? "true" : "false")
				+ ",\"evtD\":" + (evtD ? "true" : "false")
				+ ",\"evtLeft\":" + (evtLeft ? "true" : "false")
				+ ",\"evtRight\":" + (evtRight ? "true" : "false")
				+ ",\"canSteer\":" + (canSteer ? "true" : "false")
				+ ",\"state\":\"" + state + "\""
				+ ",\"grounded\":" + (_grounded ? "true" : "false")
				+ ",\"vx\":" + to_string(v.x) + ",\"vy\":" + to_string(v.y)
				+ ",\"px\":" + to_string(pos.x) + ",\"py\":" + to_string(pos.y)
				+ ",\"mass\":" + to_string(_body->GetMass())
				+ ",\"gscale\":" + to_string(_body->GetGravityScale())
				+ "}");
		}
	}
	// #endregion

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