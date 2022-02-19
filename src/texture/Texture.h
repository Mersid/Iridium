#pragma once

#include <vector>

class Texture
{
public:
	Texture(int width, int height);

	/**
	 * Sets the color of the specified pixel. The top-left corner is 0, 0
	 * @param x
	 * @param y
	 * @param color The #RRGGBBAA of the pixel. For example, 0x00FF00FF is pure green.
	 */
	void setPixel(int x, int y, unsigned int color);

	/**
	 * Sets the color of the specified pixel. The top-left corner is 0, 0
	 */
	void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

	[[nodiscard]] int getWidth() const;
	[[nodiscard]] int getHeight() const;

	[[nodiscard]] const std::vector<unsigned char> &getData() const;

private:
	/**
	 * Four bytes, RGBA, for each pixel.
	 */
	std::vector<unsigned char> data;
	int width;
	int height;
};

