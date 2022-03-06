#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "StbImplementation.h"
#include "Texture.h"


void StbImplementation::writePNG(const std::string& file, int width, int height, std::vector<unsigned char> bytes)
{
	// stride_bytes is the number of bytes in one row. Since each pixel is four bytes, it should be four bytes times the width of the row.
	stbi_write_png(file.c_str(), width, height, 4, bytes.data(), width * 4);
}

Texture StbImplementation::readPNG(const std::string& file)
{
	int width, height, channels;
	unsigned char* image = stbi_load(file.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	Texture t(width, height);
	std::vector<unsigned char> data(image, image + width * height * channels);

	t.setData(data);

	return t;
}