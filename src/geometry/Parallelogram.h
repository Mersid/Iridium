#pragma once

#include "Eigen/Core"
#include "Eigen/Dense"
#include "Ray.h"
#include "Primitive.h"
#include <optional>

/**
 * Represents a parallelogram. See https://www.desmos.com/calculator/jgxwahrilj for a 2D example of its parameterization, or
 * https://www.desmos.com/calculator/tzhhydtblv for a 3D equivalent. See sources.
 */
class Parallelogram : public Primitive
{
public:

	/**
	 * @param a The base point of the parallelogram
	 * @param b The second point of the parallelogram. Consider the cross product will use the vector ab as its A value (for A X B)
	 * @param c The third point of the parallelogram. Consider the cross product will use the vector ac as its B value (for A X B)
	 * @param diffuseCoefficient The diffuse coefficient for RGB
	 * @param specularCoefficient The specular coefficient for RGB
	 * @param phongExponent Phong exponent for specular shading. Larger number means smaller, brighter point
	 */
	Parallelogram(Eigen::Vector3d a, Eigen::Vector3d b, Eigen::Vector3d c,
	              Eigen::Vector3d diffuseCoefficient = Eigen::Vector3d(0.5, 0.5, 0.5),
	              Eigen::Vector3d specularCoefficient = Eigen::Vector3d(0.5, 0.5, 0.5),
	              double phongExponent = 10,
				  double reflectionCoefficient = 0);

	/**
	 * Default constructor for object initialization. Do not attempt to use, behaviour is undefined and may crash or segfault.
	 */
	Parallelogram();

	/**
	 * Test if the given ray intersects this parallelogram
	 * @param ray The ray to test against
	 * @return The vector representing the position of the intersection with the given ray if the ray hit the parallelogram
	 */
	std::optional<Eigen::Vector3d> getRayIntersection(const Ray& ray) override;

	/**
	 * Gets the normal of the parallelogram
	 * @param position Irrelevant for parallelograms
	 * @return A unit vector normal to the surface of the parallelogram
	 */
	Eigen::Vector3d getNormalAt(Eigen::Vector3d position) override;

	Eigen::Vector3d getDiffuseCoefficient() override;
	Eigen::Vector3d getSpecularCoefficient() override;
	double getPhongExponent() override;

	double getReflectionCoefficient() override;

private:
	Eigen::Vector3d a; // Common point
	Eigen::Vector3d b; // Usually the horizontal point
	Eigen::Vector3d c; // Usually the vertical point

	Eigen::Vector3d u; // Vector ab, equal to b-a
	Eigen::Vector3d v; // Vector ac, equal to c-a
	Eigen::Vector3d normal; // Normal vector of this parallelogram, equal to cross product of u x v.

	Eigen::Vector3d diffuseCoefficient;
	Eigen::Vector3d specularCoefficient;
	double phongExponent; // From slide set 5: ~100: shiny, ~1000: glossy, ~10000: mirror
	double reflectionCoefficient;
};


