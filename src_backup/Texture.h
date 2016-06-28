#ifndef __RUIN_TEXTURE_H_
#define __RUIN_TEXTURE_H_

#include <string>
#include <memory>
#include <GL3\glcorearb.h>

namespace Ruin
{
	class Texture
	{
		typedef std::unique_ptr<unsigned char*> TextureDataPtr;

		GLuint textureId;
		std::string filepath;
		int width, height;
		bool loaded;

	public:
		void loadFromDisk();

		Texture(const std::string &filepath);
	};
}

#endif