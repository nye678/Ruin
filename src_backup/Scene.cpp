#include "Scene.h"
#include "Services.h"

using namespace std;
using namespace Ruin;

Scene::Scene()
	: id(0)
{}

int Scene::getId()
{
	return id;
}

vector<EntityPtr> Scene::getActiveEntities()
{
	return entities;
}