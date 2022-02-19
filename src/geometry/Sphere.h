#pragma once

#include "Primitive.h"

class Sphere : public Primitive
{
public:

	/**
	 * Default constructor for object initialization. Do not attempt to use, behaviour is undefined and may crash or segfault.
	 */
	Sphere();

	/**
	 * Creates a sphere
	 * @param center The 3d coordinates of where the center of the sphere is
	 * @param radius The radius of the sphere, relative to the center
	 * @param diffuseCoefficient The diffuse coefficient for RGB
	 * @param specularCoefficient The specular coefficient for RGB
	 * @param phongExponent Phong exponent for specular shading. Larger number means smaller, brighter point
	 */
	Sphere(Eigen::Vector3d center, double radius,
		   Eigen::Vector3d diffuseCoefficient = Eigen::Vector3d(0.5, 0.5, 0.5),
		   Eigen::Vector3d specularCoefficient = Eigen::Vector3d(0.5, 0.5, 0.5),
		   double phongExponent = 10);

	/**
	 * Gets the near intersection of the intersection of a ray with a sphere
	 * @param ray The ray to fire at the sphere
	 * @return The 3d coordinates of the near impact of the ray, if applicable. If the ray doesn't miss or glance off the sphere,
	 * 			it will intersect at two points; we only want the first impact, and can disregard the second
	 */
	std::optional<Eigen::Vector3d> getRayIntersection(const Ray& ray) override;

	/**
	 * Gets the normal of the sphere at the given position. This is accomplished by returning a unit vector from the sphere's
	 * center to the given position
	 * @param position The position to obtain the normal at
	 * @return A unit vector normal to the surface at the given position
	 */
	Eigen::Vector3d getNormalAt(Eigen::Vector3d position) override;

	Eigen::Vector3d getDiffuseCoefficient() override;

	Eigen::Vector3d getSpecularCoefficient() override;

	double getPhongExponent() override;

private:
	Eigen::Vector3d center;
	double radius{};

	Eigen::Vector3d diffuseCoefficient;
	Eigen::Vector3d specularCoefficient;
	double phongExponent; // From slide set 5: ~100: shiny, ~1000: glossy, ~10000: mirror
};

