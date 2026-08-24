#include "engine.h"
#include "maths.h"
#include "../code/Controls.h"
#include "../code/GameState.h"
#include "system_physics.h"
#include "system_Renderer.h"
#include "system_resources.h"
#include "../code/SaveLoad.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstring>
#include <chrono>
#include <future>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <fstream>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <stdlib.h>
#endif
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
#endif

using namespace sf;
using namespace std;
Scene* Engine::_activeScene = nullptr;
std::string Engine::_gameName;

static bool loading = false;
static float loadingspinner = 0.f;
static float loadingTime;
static RenderWindow* _window;

bool Engine::_fullscreen = false;

static bool _keyDown[Keyboard::KeyCount] = {};
static bool _mouseDown[Mouse::ButtonCount] = {};

bool Engine::isKeyDown(Keyboard::Key key)
{
	if (key < 0 || key >= Keyboard::KeyCount)
	{
		return false;
	}
	return _keyDown[key];
}

bool Engine::isMouseDown(Mouse::Button button)
{
	if (button < 0 || button >= Mouse::ButtonCount)
	{
		return false;
	}
	return _mouseDown[button];
}

static Texture spritesheet;
static Sprite goblin;




static float switchtime;
static IntRect uvRect = IntRect(240, 0, 240, 240);
static sf::Vector2u currentimage;
static float totalTime;
static sf::Vector2u imagecount(8, 8);

void Loading_Load()
{
	spritesheet.loadFromFile("res/textures/Spritesheets/Goblin_spritesheet.png");
	goblin.setTexture(spritesheet);
	goblin.setTextureRect(uvRect);
	goblin.setOrigin(0, 120);
	goblin.setPosition(Engine::getWindowSize().x / 2, Engine::getWindowSize().y / 4);

	switchtime = 0.045f;

	totalTime = 0.f;

	uvRect.width = spritesheet.getSize().x / (imagecount.x);
	uvRect.width = spritesheet.getSize().y / (imagecount.y);

}

void Loading_update(float dt, const Scene* const scn) {
	Loading_Load();

	if (scn->isLoaded()) {
		loading = false;
	}

	else {

		totalTime += dt;

		if (totalTime >= switchtime)
		{
			totalTime -= switchtime;

			currentimage.x++;

			if (currentimage.x >= imagecount.x)
			{
				currentimage.x = 0;
			}
		}

		uvRect.left = currentimage.x * uvRect.width;
		uvRect.top = currentimage.y *  uvRect.height;

		goblin.setTextureRect(uvRect);
		goblin.setOrigin(Engine::GetWindow().getSize().x / 2, Engine::GetWindow().getSize().y / 2);
		goblin.setPosition(Engine::GetWindow().getSize().x - 125, Engine::GetWindow().getSize().y / 2 + 200);

	}

}
void Loading_render() {

	static Text t("Loading...", *Resources::get<sf::Font>("Wonder.ttf"));
	t.setOrigin(Engine::GetWindow().getSize().x / 2, Engine::GetWindow().getSize().y / 2);
	t.setPosition(Engine::GetWindow().getSize().x / 2, Engine::GetWindow().getSize().y / 2);

	Renderer::queue(&t);
	Renderer::queue(&goblin);
}

float frametimes[256] = {};
uint8_t ftc = 0;

void Engine::Update() {
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();
	{
		frametimes[++ftc] = dt;
		static string avg = _gameName + " FPS:";
		//if (ftc % 60 == 0) {
		double davg = 0;
		for (const auto t : frametimes) {
			davg += t;
		}
		davg = 1.0 / (davg / 255.0);
		_window->setTitle(avg + toStrDecPt(2, davg));
		//}
	}


	if (loading) {
		Loading_update(dt, _activeScene);
	}
	else if (_activeScene != nullptr) {
		Physics::update(dt);
		_activeScene->Update(dt);
	}
}

void Engine::Render(RenderWindow& window) {
	if (loading) {
		Loading_render();
	}
	else if (_activeScene != nullptr) {
		_activeScene->Render();
	}

	Renderer::render();
}


void Engine::UseExecutableWorkingDirectory() {
#ifdef _WIN32
	char path[MAX_PATH];
	DWORD n = GetModuleFileNameA(nullptr, path, MAX_PATH);
	if (n == 0 || n >= MAX_PATH) {
		return;
	}
	std::string dir(path);
	const auto slash = dir.find_last_of("\\/");
	if (slash != std::string::npos) {
		dir.resize(slash);
		SetCurrentDirectoryA(dir.c_str());
	}
#elif defined(__APPLE__)
	char path[PATH_MAX];
	uint32_t size = sizeof(path);
	if (_NSGetExecutablePath(path, &size) != 0) {
		return;
	}
	char resolved[PATH_MAX];
	if (realpath(path, resolved) == nullptr) {
		return;
	}
	std::string dir(resolved);
	const auto slash = dir.find_last_of('/');
	if (slash != std::string::npos) {
		dir.resize(slash);
		chdir(dir.c_str());
	}
#else
	char path[PATH_MAX];
	const ssize_t n = readlink("/proc/self/exe", path, sizeof(path) - 1);
	if (n <= 0) {
		return;
	}
	path[n] = '\0';
	std::string dir(path);
	const auto slash = dir.find_last_of('/');
	if (slash != std::string::npos) {
		dir.resize(slash);
		chdir(dir.c_str());
	}
#endif
}

void Engine::Start(unsigned int width, unsigned int height,
	const std::string& gameName, Scene* scn, unsigned int frameRate) {
	UseExecutableWorkingDirectory();
	RenderWindow window;
	window.create(VideoMode(width, height, 2), gameName);
	window.setFramerateLimit(frameRate);
	window.requestFocus();
	//window.create(VideoMode(width, height), gameName, Style::Titlebar | Style::Close);
	_gameName = gameName;
	_window = &window;
	Renderer::initialise(window);

	Physics::initialise();

	//FOR WHEN I HAVE CONTROLS WORKING

	Controls::initialise();

	SaveLoad::ResetGame();
	ChangeScene(scn);
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			if (event.type == Event::LostFocus)
			{
				memset(_keyDown, 0, sizeof(_keyDown));
				memset(_mouseDown, 0, sizeof(_mouseDown));
			}
			if (event.type == Event::KeyPressed && event.key.code >= 0 && event.key.code < Keyboard::KeyCount)
			{
				_keyDown[event.key.code] = true;
			}
			if (event.type == Event::KeyReleased && event.key.code >= 0 && event.key.code < Keyboard::KeyCount)
			{
				_keyDown[event.key.code] = false;
			}
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button >= 0 && event.mouseButton.button < Mouse::ButtonCount)
			{
				_mouseDown[event.mouseButton.button] = true;
			}
			if (event.type == Event::MouseButtonReleased && event.mouseButton.button >= 0 && event.mouseButton.button < Mouse::ButtonCount)
			{
				_mouseDown[event.mouseButton.button] = false;
			}
			//Resize Window Mode
			if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visableArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visableArea));
			}
			//Fullscreen Mode
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::F11)
				{
					_fullscreen = !_fullscreen;
					window.create(VideoMode(width, height), gameName, (_fullscreen ? Style::Fullscreen : Style::Resize | Style::Close));
					window.setFramerateLimit(frameRate);
				    _window = &window;
					window.requestFocus();
					memset(_keyDown, 0, sizeof(_keyDown));
					memset(_mouseDown, 0, sizeof(_mouseDown));
				}
			}
		}

		window.clear(Color::Black);
		Update();
		Render(window);
		window.display();
	}
	if (_activeScene != nullptr) {
		_activeScene->UnLoad();
		_activeScene = nullptr;
	}

	window.close();
	Physics::shutdown();
}

std::shared_ptr<Entity> Scene::makeEntity() {
	auto e = make_shared<Entity>(this);
	ents.list.push_back(e);
	return std::move(e);
}


void Engine::setVsync(bool b) { _window->setVerticalSyncEnabled(b); }

void Engine::ChangeScene(Scene* s) {
	auto old = _activeScene;
	_activeScene = s;

	if (old != nullptr) {
		old->UnLoad(); // todo: Unload Async
	}

	if (!s->isLoaded()) {
		loadingTime = 0;
		_activeScene->LoadAsync();
		loading = true;
	}
}

void Scene::Update(const double& dt) { ents.update(dt); }

void Scene::Render() { ents.render(); }

bool Scene::isLoaded() const {
	{
		std::lock_guard<std::mutex> lck(_loaded_mtx);
		// Are we already loading asynchronously?
		if (_loaded_future.valid() // yes
			&&                     // Has it finished?
			_loaded_future.wait_for(chrono::seconds(0)) ==
			future_status::ready) {
			// Yes
			_loaded_future.get();
			_loaded = true;
		}
		return _loaded;
	}
}
void Scene::setLoaded(bool b) {
	{
		std::lock_guard<std::mutex> lck(_loaded_mtx);
		_loaded = b;
	}
}

void Scene::UnLoad() {
	ents.list.clear();
	setLoaded(false);
}

void Scene::LoadAsync() { _loaded_future = std::async(&Scene::Load, this); }

sf::Vector2u Engine::getWindowSize() { return _window->getSize(); }

sf::RenderWindow& Engine::GetWindow() { return *_window; }

namespace timing {
	// Return time since Epoc
	long long now() {
		return std::chrono::high_resolution_clock::now()
			.time_since_epoch()
			.count();
	}
	// Return time since last() was last called.
	long long last() {
		auto n = now();
		static auto then = now();
		auto dt = n - then;
		then = n;
		return dt;
	}
} // namespace timing

Scene::~Scene() { UnLoad(); }

//get active scene for prefabs
Scene* Engine::GetActiveScene()
{
	return _activeScene;
}