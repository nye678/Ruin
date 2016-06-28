#ifndef __RUIN_MODEL_H_
#define __RUIN_MODEL_H_

#include <string>
#include <vector>
#include <glm\glm.hpp>

class Model
{
	unsigned int _id;

	std::vector<glm::vec3> _vertices;
	std::vector<glm::vec3> _normals;
	std::vector<glm::uvec3> _faces;
	std::vector<unsigned int> _faceNormalIndices;
	bool _smoothShading;

public:

	static Model LoadFromObj(const char* filename);
};

#endif