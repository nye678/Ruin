#ifndef __RUIN_MTL_FILE_H_
#define __RUIN_MTL_FILE_H_

#include <string>
#include <glm\glm.hpp>

namespace Ruin {
namespace FileTypes {

	enum MtlIlluminationModel
	{
		ColorOnAmbientOff					= 0,
		ColorOnAmbientOn					= 1,
		HighlightOn							= 2,
		ReflectionOnRayTraceOn				= 3,
		GlassOnReflectionRayTraceOn			= 4,
		FresnelOnRayTraceOn					= 5,
		RefractionOnFresnelOffRayTraceOn	= 6,
		RefractionOnFresnelOnRayTraceOn		= 7,
		ReflectionOnRayTraceOff				= 8,
		GlassOnRayTraceOff					= 9,
		ShadowsOntoInvisibiles				= 10
	};

	struct MtlFile
	{
		std::string Name;
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		float SpecularCoefficent;
		float Transparency;
		MtlIlluminationModel IlluminationModel;
	};

}}

#endif