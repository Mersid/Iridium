#include "PerlinNoiseSampler.h"

Eigen::Vector2d hashToVec(int hash, int x, int y)
{
	switch(hash & 0x5)
	{
		case 0x0: return  Eigen::Vector2d(1, 1);
		case 0x1: return  Eigen::Vector2d(-1, 1);
		case 0x2: return  Eigen::Vector2d(1, -1);
		case 0x3: return  Eigen::Vector2d(-1, -1);
		case 0x4: return  Eigen::Vector2d(1, 1);
		case 0x5: return  Eigen::Vector2d(1, -1);
		default: return Eigen::Vector2d(0, 0); // never happens
	}
}

Eigen::Vector2d PerlinNoiseSampler::getGradientVectorAt(int x, int y)
{
//	std::hash<uint64_t> hasher;
//
//	uint64_t hashable = x;
//
//	uint64_t hash = hasher((hashable << 32) | y);
//
//	unsigned int hashX = (hash >> 32);
//	unsigned int hashY = hash & 0xFFFFFFFF;
//
//	return Eigen::Vector2d(
//			(double)hashX / std::numeric_limits<unsigned int>::max(),
//			(double)hashY / std::numeric_limits<unsigned int>::max()).normalized();



//	int permutation[256] = {151, 160, 137, 91, 90,
//	                        15,                    // Hash lookup table as defined by Ken Perlin.  This is a randomly
//	                        131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21,
//	                        10, 23,    // arranged array of all numbers from 0-255 inclusive.
//	                        190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177,
//	                        33,
//	                        88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27,
//	                        166,
//	                        77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40,
//	                        244,
//	                        102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200,
//	                        196,
//	                        135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124,
//	                        123,
//	                        5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189,
//	                        28, 42,
//	                        223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
//	                        129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
//	                        228,
//	                        251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
//	                        107,
//	                        49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
//	                        138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
//	};
//
//	int p[512];
//
//	for (int x = 0; x < 512; x++)
//	{
//		p[x] = permutation[x % 256];
//	}
//
//	int hash = p[p[x] + y];
//	return hashToVec(hash, x, y);

	return vectors[(y % 256) * 256 + (x % 256)];
}

double PerlinNoiseSampler::sample(double x, double y)
{
	Eigen::Vector2d p(x, y);

	// Compute the lower left values of the box that (x, y) occupies. We can find the upper right by adding 1 to the lower left's values.
	int lowerLeftX = std::floor(x);
	int lowerLeftY = std::floor(y);
	int upperRightX = lowerLeftX + 1;
	int upperRightY = lowerLeftY + 1;

	// Compute the offset of the point within the box Should be between 0 and 1.
	Eigen::Vector2d localP(p[0] - lowerLeftX, p[1] - lowerLeftY);

	// Compute the random gradient vectors for each corner. Let g0 be the upper left, g1 upper right, g2 lower left, and g3 lower right corners.
	// The g stands for gradient
	Eigen::Vector2d g0 = getGradientVectorAt(lowerLeftX, upperRightY);
	Eigen::Vector2d g1 = getGradientVectorAt(upperRightX, upperRightY);
	Eigen::Vector2d g2 = getGradientVectorAt(lowerLeftX, lowerLeftY);
	Eigen::Vector2d g3 = getGradientVectorAt(upperRightX, lowerLeftY);

	// For each corner, compute the vector from it to the sampled point. Same convention as above, but let d be the direction vector thereof.
	Eigen::Vector2d d0 = (localP - Eigen::Vector2d(0, 1));
	Eigen::Vector2d d1 = (localP - Eigen::Vector2d(1, 1));
	Eigen::Vector2d d2 = (localP);
	Eigen::Vector2d d3 = (localP - Eigen::Vector2d(1, 0));

	// Compute the dot value for each corner. If we have a sampled point on an integer value, the vector opposite it will have
	// a length of 1 + 1 (imagine a triangle), so length is sqrt 2. This is the max possible length before the value is "passed on"
	// to the next grid. We can divide by sqrt 2 to normalize the values to +/- 1
	double v0 = g0.dot(d0) / std::sqrt(2);
	double v1 = g1.dot(d1) / std::sqrt(2);
	double v2 = g2.dot(d2) / std::sqrt(2);
	double v3 = g3.dot(d3) / std::sqrt(2);


	// Compute the average-of-averages of the dot values
	double avg0 = lerp(v0, v1, smooth(localP[0]));
	double avg1 = lerp(v2, v3, smooth(localP[0]));

	double aoa = lerp(avg1, avg0, smooth(localP[1]));

	return aoa;
}

double PerlinNoiseSampler::lerp(double from, double to, double weight)
{
	return from + (to - from) * weight;
}

double PerlinNoiseSampler::smooth(double in)
{
	return in * in * in * (in * (in * 6.0 - 15.0) + 10.0);
}

PerlinNoiseSampler::PerlinNoiseSampler()
{
	mt = std::mt19937();
	dist = std::uniform_real_distribution<double>(-1.0, 1.0);

	vectors.reserve(256 * 256);
	for (int i = 0; i < vectors.capacity(); i++)
	{
		vectors.emplace_back(Eigen::Vector2d(dist(mt), dist(mt)).normalized());
	}
}

