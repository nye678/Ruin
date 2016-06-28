#ifndef __RUIN_MATERIAL_H_
#define __RUIN_MATERIAL_H_

namespace Ruin
{
	class Material
	{
		unsigned int matrialId;
		unsigned int shaderProgram;
		unsigned int textureId;

	public:
		unsigned int getId();

		unsigned int getShaderProgram();
		void setShaderProgram(unsigned int);

		unsigned int getTexture();
		void setTexture(unsigned int);

		void enableMaterial();

		Material(unsigned int id);
	};
}

#endif