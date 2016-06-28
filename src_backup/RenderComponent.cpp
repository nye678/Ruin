#include "RenderComponent.h"
#include <GLFW\glfw3.h>

using namespace std;
using namespace glm;
using namespace Ruin;

const char* RenderComponent::RenderComponentTypeName = "RenderComponent";

RenderComponent::RenderComponent(EntityPtr entity)
	: Component(entity)
{
	modelViewMatrix = glm::mat4(1.0f);
}

void RenderComponent::update(double deltaTime)
{
	auto orientation = entity->getOrientation();
	entity->setOrientation(rotate(orientation, 5.0f, vec3(1.0, 1.0, 1.0)));

	modelViewMatrix = mat4(1.0f);
	modelViewMatrix = glm::translate(modelViewMatrix, entity->getPosition());
	modelViewMatrix = translate(modelViewMatrix, vec3(0.0f, 0.0f, -20.0f));
	modelViewMatrix *= mat4_cast(entity->getOrientation());
	modelViewMatrix = scale(modelViewMatrix, entity->getScale());
}

unsigned int RenderComponent::getSortKey()
{
	return 0;
}

void RenderComponent::draw(RendererPtr renderer)
{
	renderer->setMaterial(material);
	renderer->setModel(modelHandle);
	renderer->setModelViewMatrix(modelViewMatrix);
	renderer->drawElements(0);
}

string RenderComponent::getTypeName()
{
	return string(RenderComponentTypeName);
}

void RenderComponent::setModel(unsigned int modelHandle)
{
	this->modelHandle = modelHandle;
}

void RenderComponent::setMaterial(unsigned int materialId)
{
	this->material = materialId;
}