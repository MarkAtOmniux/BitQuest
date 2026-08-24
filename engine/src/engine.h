#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <ecm.h>
#include <future>
#include <math.h>
#include <mutex>
#include <string>

class Scene {
public:
	Scene() = default;
	virtual ~Scene();
	virtual void Load() = 0;
	virtual void LoadAsync();
	virtual void UnLoad();
	virtual void Update(const double& dt);
	virtual void Render();
	bool isLoaded() const;
	std::shared_ptr<Entity> makeEntity();

	EntityManager ents;

protected:
	void setLoaded(bool);
private:
	mutable bool _loaded;
	mutable std::future<void> _loaded_future;
	mutable std::mutex _loaded_mtx;
};


class Engine {
public:
	Engine() = delete;
	static void Start(unsigned int width, unsigned int height,
	const std::string& gameName, Scene* scn, unsigned int frameRate);
	// Make relative "res/..." paths work when launched from Finder or Explorer.
	static void UseExecutableWorkingDirectory();
	static void ChangeScene(Scene*);
	static sf::RenderWindow& GetWindow();
	static sf::Vector2u getWindowSize();
	static void setVsync(bool b);
	static Scene* GetActiveScene();
	static bool isKeyDown(sf::Keyboard::Key key);
	static bool isMouseDown(sf::Mouse::Button button);


private:
	static Scene* _activeScene;
	static std::string _gameName;
	static void Update();
	static void Render(sf::RenderWindow& window);
	static bool _fullscreen;

};

namespace timing {
	// Return time since Epoc
	long long now();
	// Return time since last() was last called.
	long long last();
}