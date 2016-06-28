#include <cassert>
#include "SceneManager.h"

using namespace std;
using namespace Ruin;

void SceneManager::addScene(ScenePtr scene)
{
	scenes.insert(make_pair(scene->getId(), scene));
}

void SceneManager::loadScene(int sceneId)
{
}

void SceneManager::switchScene(int sceneId)
{
	currentScene = scenes.at(sceneId);
}

ScenePtr SceneManager::getCurrent()
{
	assert(currentScene != nullptr);
	return currentScene;
}