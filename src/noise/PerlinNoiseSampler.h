#pragma once

#include "Eigen/Core"

class PerlinNoiseSampler
{
public:
	double sample(double x, double y);

private:
	Eigen::Vector2d getGradientVectorAt(int x, int y);
	double lerp(double value, double from, double to); // TODO: Move this, maybe
};
