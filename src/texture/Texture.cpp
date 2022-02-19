#include "Texture.h"

Texture::Texture(int width, int height) : width(width), height(height)
{
	data = std::vector<unsigned char>(width * height * 4, 255);

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
	int offset = (y * width + x) * 4;

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

