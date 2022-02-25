#pragma once


#include <optional>
#include "Eigen/Core"
#include "Ray.h"

class Primitive
{
public:
	/**
	 * Test if the given ray intersects this parallelogram
	 * @param ray The ray to test against
	 * @return The vector representing the position of the intersection with the given ray if the ray hit the parallelogram
	 */
	virtual std::optional<Eigen::Vector3d> getRayIntersection(const Ray& ray) = 0;

	/**
	 * Gets the vector normal to the surface of the primitive at the specified position. The position should be close to the surface, or undefined
	 * behaviour may occur
	 * @param position The position to find the normal of
	 * @return A unit vector normal to the surface of the primitive at the specified position
	 */
	virtual Eigen::Vector3d getNormalAt(Eigen::Vector3d position) = 0;

	virtual Eigen::Vector3d getDiffuseCoefficient() = 0;
	virtual Eigen::Vector3d getSpecularCoefficient() = 0;
	virtual double getPhongExponent() = 0;
	virtual double getReflectionCoefficient() = 0;
};


