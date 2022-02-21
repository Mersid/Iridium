#include "PerlinNoiseSampler.h"

Eigen::Vector2d PerlinNoiseSampler::getGradientVectorAt(int x, int y)
{
	std::hash<uint64_t> hasher;

	uint64_t x64 = x;
	uint64_t y64 = y;

	unsigned int hashX = hasher((x64 << 32) | y64);
	unsigned int hashY = hasher((y64 << 32) | x64);

	return Eigen::Vector2d(
			(double)hashX / std::numeric_limits<unsigned int>::max(),
			(double)hashY / std::numeric_limits<unsigned int>::max()).normalized();
}

double PerlinNoiseSampler::sample(double x, double y)
{
	Eigen::Vector2d p(x, y);

	// Compute the lower left values of the box that (x, y) occupies. We can find the upper right by adding 1 to the lower left's values.
	int lowerLeftX = std::floor(x);
	int lowerLeftY = std::floor(y);
	int upperRightX = lowerLeftX + 1;
	int upperRightY = lowerLeftY + 1;

	// Compute the random gradient vectors for each corner. Let g0 be the upper left, g1 upper right, g2 lower left, and g3 lower right corners.
	// The g stands for gradient
	Eigen::Vector2d g0 = getGradientVectorAt(lowerLeftX, upperRightY);
	Eigen::Vector2d g1 = getGradientVectorAt(upperRightX, upperRightY);
	Eigen::Vector2d g2 = getGradientVectorAt(lowerLeftX, lowerLeftY);
	Eigen::Vector2d g3 = getGradientVectorAt(upperRightX, lowerLeftY);

	// For each corner, compute the vector from it to the sampled point. Same convention as above, but let d be the direction vector thereof.
	Eigen::Vector2d d0 = p - g0;
	Eigen::Vector2d d1 = p - g1;
	Eigen::Vector2d d2 = p - g2;
	Eigen::Vector2d d3 = p - g3;

	// Compute the dot value for each corner.
	double v0 = g0.dot(d0);
	double v1 = g1.dot(d1);
	double v2 = g2.dot(d2);
	double v3 = g3.dot(d3);

	// Compute the offset of the point within the box Should be between 0 and 1.
	Eigen::Vector2d localP(p[0] - lowerLeftX, p[1] - lowerLeftY);

	// Compute the average-of-averages of the dot values
	double avg0 = lerp(v0, v1, localP[0]);
	double avg1 = lerp(v2, v3, localP[0]);

	double aoa = lerp(avg0, avg1, localP[1]);

	return aoa;
}

double PerlinNoiseSampler::lerp(double value, double from, double to)
{
	return from + (to - from) * value;
}

