#pragma once


#include "Eigen/Core"

class Box
{
public:
	Box(const Eigen::Vector3d& min, const Eigen::Vector3d& max);
	Box() = default;

	[[nodiscard]] Eigen::Vector3d& getMin();

	[[nodiscard]] Eigen::Vector3d& getMax();


private:
	Eigen::Vector3d min;
	Eigen::Vector3d max;
};


