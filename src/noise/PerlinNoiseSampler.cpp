#include "PerlinNoiseSampler.h"

Eigen::Vector2d PerlinNoiseSampler::getGradientVectorAt(int x, int y)
{
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
	//If we have a sampled point on an integer value, the vector opposite it will have
	// a length of 1 + 1 (imagine a triangle), so length is sqrt 2. This is the max possible length before the value is "passed on"
	// to the next grid. We can divide by sqrt 2 to normalize the values to +/- 1
	Eigen::Vector2d d0 = (localP - Eigen::Vector2d(0, 1)) / sqrt(2);
	Eigen::Vector2d d1 = (localP - Eigen::Vector2d(1, 1)) / sqrt(2);
	Eigen::Vector2d d2 = (localP) / sqrt(2);
	Eigen::Vector2d d3 = (localP - Eigen::Vector2d(1, 0)) / sqrt(2);

	// Compute the dot value for each corner.
	// Aside: We can also do the sqrt division here for the same results. It intuitively makes sense, I suppose,
	// but it makes more sense to do the vector division above.
	double v0 = g0.dot(d0);
	double v1 = g1.dot(d1);
	double v2 = g2.dot(d2);
	double v3 = g3.dot(d3);


	// Compute the average-of-averages of the dot values
	double avg0 = lerp(v0, v1, smooth(localP[0]));
	double avg1 = lerp(v2, v3, smooth(localP[0]));

	double aoa = lerp(avg1, avg0, smooth(localP[1]));

	// Ok so I just wrote a short little "proof".
	// Consider a grid where each random vector faces directly AWAY from the center of the grid.
	// Let's say our random point is in the bottom-right corner (local coordinates approaching 0, 1).
	// Now, note that each random vector is unit length, and each dot product was scaled by sqrt 2,
	// so its max length is also unit (1). The scaling is linear, so it should work out.
	// Now, in the top-left corner, the dot value will be -1, top-right is -1/sqrt(2), bottom left (origin) -1/sqrt(2)
	// and bottom right, where the dot is close to, the dot product approaches 0 (let's say it is).
	// Then the lerp for avg0 is between -1 and -1/sqrt(2), scaled entirely towards -1/sqrt(2) because the weight approaches 1,
	// the lerp for avg1 is between -1/sqrt(2) and 0, scaled towards 0 because the weight approaches 1 (recall, dot is on bottom right, so x ~ 1)1
	// Now, the lerp for the top part is -1/sqrt(2), and the lerp for the bottom is 0. We are scaling from bottom to top (for continuity reasons)
	// so the aoa is lerp between 0 and -/sqrt(2). Now, because that dot is touching toe bottom, the weight approaches 0, so it's scaled towards 0.
	// Thus, the value is 0, and intuitively makes sense why the min is 0.

	return aoa; // I'm not really sure why it's automatically returning positive values only, but it works just fine! NVM SEE ABOVE
}

double PerlinNoiseSampler::lerp(double from, double to, double weight)
{
	return from + (to - from) * weight;
	//return (to - from) * (3.0 - weight * 2.0) * weight * weight + from; // Cubic interpolation
}

double PerlinNoiseSampler::smooth(double in)
{
	return in * in * in * (in * (in * 6.0 - 15.0) + 10.0);
}

PerlinNoiseSampler::PerlinNoiseSampler()
{
	std::mt19937 mt = std::mt19937();
	std::uniform_real_distribution<double> dist = std::uniform_real_distribution<double>(-1.0, 1.0);

	vectors.reserve(256 * 256);
	for (int i = 0; i < vectors.capacity(); i++)
	{
		vectors.emplace_back(Eigen::Vector2d(dist(mt), dist(mt)).normalized());
	}
}

