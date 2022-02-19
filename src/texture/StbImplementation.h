#pragma once

#include <string>
#include <vector>

/**
 * Implementations for stb_image and related headers. The implementation itself is in the associated .cpp file to ensure it is
 * defined only once through the entire project.
 */
class StbImplementation
{
public:
	/**
	 * Writes pixels to file. The existing file will be overwritten.
	 * @param file The file to write the image to, in PNG RGBA format.
	 * @param width The width of the image
	 * @param height The height of the image.
	 * @param bytes The raw bytes to write to the image. It should be a multiple of four bytes, where the first byte
	 * 				is red, second is green, third is blue, and fourth is alpha. Values should be between 0 and 255,
	 * 				and the number of bytes should be 4 * width * height.
	 */
	static void writePNG(const std::string& file, int width, int height, std::vector<unsigned char> bytes);
};


