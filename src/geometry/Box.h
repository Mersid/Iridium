#pragma once


#include "Eigen/Core"
#include "Ray.h"

class Box
{
public:
	Box(Eigen::Vector3d min, Eigen::Vector3d max);
	Box() = default;

	/**
	 * @param boxes A list of boxes to aggregate
	 * @return A box large enough to contain all boxes in the vector
	 */
	static Box aggregate(const std::vector<Box>& boxes);

	[[nodiscard]] Eigen::Vector3d& getMin();

	[[nodiscard]] Eigen::Vector3d& getMax();

	// Gets the center of the box
	Eigen::Vector3d getPosition();

	/**
	 * Checks if the ray intersects the box. Because the box is not meant to be used as a rendering primitive, it isn't one,
	 * and many features, like normal detection and vector coordinates of hit positions, are not available. It's simply meant
	 * to determine if the ray hit or not, nothing more.
	 * @param ray The ray to fire against the box
	 * @return True if it hit, false otherwise
	 */
	bool intersect(const Ray& ray);


private:
	Eigen::Vector3d min;
	Eigen::Vector3d max;
};


