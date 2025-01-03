#pragma once

namespace Based {

class Scene {
public:
	virtual void handle_events () = 0;
	virtual void update () = 0;
	virtual void render () = 0;
};

struct SceneManager {
	Scene *currentScene = nullptr;
	void handle_events ();
	void update ();
	void render ();
};

}
