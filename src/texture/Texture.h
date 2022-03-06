#pragma once

#include <vector>
#include "Eigen/Core"

class Texture
{
public:
	Texture(int width, int height);

	/**
	 * Sets the color of the specified pixel. The top-left corner is 0, 0
	 * @param color The #RRGGBBAA of the pixel. For example, 0x00FF00FF is pure green.
	 */
	void setPixel(int x, int y, unsigned int color);

	/**
	 * Sets the color of the specified pixel. The top-left corner is 0, 0
	 */
	void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

	/**
	 * @param data Replace the data in this texture with the data passed in from this parameter
	 */
	void setData(const std::vector<unsigned char>& d);

	[[nodiscard]] int getWidth() const;
	[[nodiscard]] int getHeight() const;

	/**
	 * @return A normalized color (0, 1) vector at the given coordinates. Is a utility function; if we need the true color, get the actual data and compute it there.
	 * Returns in the form RGBA.
	 */
	Eigen::Vector4d getPixelVectorAt(int x, int y) const;

	[[nodiscard]] const std::vector<unsigned char>& getData() const;

private:
	/**
	 * Four bytes, RGBA, for each pixel.
	 */
	std::vector<unsigned char> data;
	int width;
	int height;
};

