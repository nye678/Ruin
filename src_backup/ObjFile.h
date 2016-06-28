#ifndef __RUIN_OBJ_FILE_H_
#define __RUIN_OBJ_FILE_H_

#include <string>
#include <vector>
#include <glm\glm.hpp>
#include "MtlFile.h"

namespace Ruin {
namespace FileTypes {

	struct ObjFileObject
	{
		std::string Name;
		std::vector<glm::vec3> Vertices;
		std::vector<glm::vec3> Normals;
		std::vector<std::vector<unsigned int>> Faces;
		std::vector<unsigned int> FaceNormalIndices;
		bool SmoothShading;
		MtlFile* Material;
	};

	struct ObjFile
	{
		std::vector<ObjFileObject> Objects;
		std::vector<MtlFile> Materials;

		ObjFileObject* CurrentObject;

		std::string Filename;
	};

}}

#endif