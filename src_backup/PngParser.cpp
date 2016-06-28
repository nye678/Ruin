#include "PngParser.h"
#include "Utility.h"

#include <cassert>
#include <libpng\png.h>

using namespace std;
using namespace Ruin;
using namespace Ruin::FileTypes;

PngFile PngParser::ParsePngFile(const string &filepath)
{
	FILE* fp;
	errno_t error = fopen_s(&fp, filepath.c_str(), "rb");
	assert(fp);
	
	png_byte header[8];
	fread_s(header, 8, sizeof(png_byte), 8, fp);
	int isPng = !png_sig_cmp(header, 0, 8);
	assert(isPng);
	
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	assert(png_ptr);

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, nullptr, nullptr);
		assert(false);
	}

	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, nullptr, nullptr);
		assert(false);
	}
	
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		assert(false);
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	int width = png_get_image_width(png_ptr, info_ptr);
	int height = png_get_image_height(png_ptr, info_ptr);
	png_byte color_type = png_get_color_type(png_ptr, info_ptr);
	png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	int rowBytes = png_get_rowbytes(png_ptr, info_ptr);

	rowBytes += 3 - ((rowBytes - 1) % 4);

	png_byte* image_data;
	image_data = (png_byte*)malloc(rowBytes * height * sizeof(png_byte) + 15);
	assert(image_data);

	png_bytep* row_pointers = (png_bytep*)malloc(height * sizeof(png_bytep));
	assert(row_pointers);

	for (int i = 0; i < height; i++)
	{
		row_pointers[height - 1 - i] = image_data + i * rowBytes;
	}

	png_read_image(png_ptr, row_pointers);

	// Generate the OpenGL texture object
	//GLuint texture;
	//glGenTextures(1, &texture);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// clean up
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	free(image_data);
	free(row_pointers);
	fclose(fp);
	
	return PngFile();
}