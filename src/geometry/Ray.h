#pragma once

#include "Eigen/Core"

class Ray
{
public:

	/**
	 * Constructs a ray at the origin, with no direction (0, 0, 0). Should be used only as a placeholder
	 */
	Ray();

	/**
	 * Constructs a ray with the given position and pos2
	 * @param position The initial position of the ray
	 * @param pos2 Another point on the ray. The ray is formed from the initial position, going through this point.
	 * 			   This is convenient because we can use a direction vector, or a "look-at" vector that will get automatically normalized.
	 */
	Ray(const Eigen::Vector3d& position, const Eigen::Vector3d& pos2);

	[[nodiscard]] const Eigen::Vector3d& getPosition() const;

	/**
	 * @return A unit vector describing the direction of the ray
	 */
	[[nodiscard]] const Eigen::Vector3d& getDirection() const;

private:
	Eigen::Vector3d position;
	Eigen::Vector3d direction; // Doesn't actually have to be a unit vector
};
