#pragma once

#include <random>
#include "Eigen/Core"
#include "PerlinFilterMode.h"

class PerlinNoiseSampler
{
public:
	explicit PerlinNoiseSampler(PerlinFilterMode mode = PerlinFilterMode::FADE);
	double sample(double x, double y);

private:
	Eigen::Vector2d getGradientVectorAt(int x, int y);
	double lerp(double from, double to, double weight);
	double smooth(double in);

	PerlinFilterMode mode;
	std::vector<Eigen::Vector2d> vectors;
};
