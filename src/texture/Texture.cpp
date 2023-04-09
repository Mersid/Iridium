#include "Texture.h"

Texture::Texture(int width, int height) : width(width), height(height)
{
	data = std::vector<unsigned char>((uint64_t)width * height * 4, 255);

	// Set alpha to 255, so default texture is black
	for (std::vector<unsigned char>::size_type i = 0; i < data.size(); i++)
		if (i % 4 == 3)
			data[i] = 255;
}

void Texture::setPixel(int x, int y, unsigned int color)
{
	unsigned char r = (color >> 24) & 0xFF;
	unsigned char g = (color >> 16) & 0xFF;
	unsigned char b = (color >> 8) & 0xFF;
	unsigned char a = color & 0xFF;

	setPixel(x, y, r, g, b, a);
}

void Texture::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	uint64_t offset = ((uint64_t)y * width + x) * 4;

	data[offset] = r;
	data[offset + 1] = g;
	data[offset + 2] = b;
	data[offset + 3] = a;
}


int Texture::getWidth() const
{
	return width;
}

int Texture::getHeight() const
{
	return height;
}

const std::vector<unsigned char>& Texture::getData() const
{
	return data;
}

Eigen::Vector4d Texture::getPixelVectorAt(int x, int y) const
{
	unsigned long long offset = ((unsigned long long )y * width + x) * 4;

	unsigned char r = data[offset] ;
	unsigned char g = data[offset + 1];
	unsigned char b = data[offset + 2];
	unsigned char a = data[offset + 3];

	return Eigen::Vector4d(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
}

void Texture::setData(const std::vector<unsigned char>& d)
{
	data = d;
}

