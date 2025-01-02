#pragma once

namespace Based {

class Scene {
public:
	virtual void update () = 0;
	virtual void render () = 0;
	virtual void handleEvents () = 0;
};

class SceneManager {
public:
	Scene *currentScene = nullptr;
	void update ();
	void render ();
	void handleEvents ();
};

#ifndef _BASED_IMPLEMENT
extern SceneManager sceneManager;
#endif

}
