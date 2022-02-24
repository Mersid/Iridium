#pragma once

#include <random>
#include "Eigen/Core"

class PerlinNoiseSampler
{
public:
	PerlinNoiseSampler();
	double sample(double x, double y);

private:
	Eigen::Vector2d getGradientVectorAt(int x, int y);
	double lerp(double from, double to, double weight); // TODO: Move this, maybe
	double smooth(double in);

	std::mt19937 mt;
	std::uniform_real_distribution<double> dist;

	std::vector<Eigen::Vector2d> vectors;
};
