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
	 * Constructs a ray with the given position and direction
	 * @param position The initial position of the ray
	 * @param direction The direction of the ray. It will be normalized automatically.
	 */
	Ray(Eigen::Vector3d position, Eigen::Vector3d direction);

	[[nodiscard]] const Eigen::Vector3d& getPosition() const;

	/**
	 * @return A unit vector describing the direction of the ray
	 */
	[[nodiscard]] const Eigen::Vector3d& getDirection() const;

private:
	Eigen::Vector3d position;
	Eigen::Vector3d direction; // Doesn't actually have to be a unit vector
};
