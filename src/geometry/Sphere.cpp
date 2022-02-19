#include "Sphere.h"

Sphere::Sphere(Eigen::Vector3d center, double radius, Eigen::Vector3d diffuseCoefficient, Eigen::Vector3d specularCoefficient, double phongExponent) :
		center(center), radius(radius),
		diffuseCoefficient(diffuseCoefficient),
		specularCoefficient(specularCoefficient),
		phongExponent(phongExponent)
{
}

std::optional<Eigen::Vector3d> Sphere::getRayIntersection(const Ray& ray)
{
	// Parameterization value of the ray at its closest point to the center of the sphere
	double rayParamNearest = (center - ray.getPosition()).dot(ray.getDirection());

	// Don't be like me and forget this. Causes some fun issue with shadow rays :P -- Steven, 2022-02-13
	if (rayParamNearest <= 0)
		return std::nullopt;

	// The point in 3d space along the ray where it is closest to the center of the sphere
	Eigen::Vector3d nearestApproach = ray.getPosition() + rayParamNearest * ray.getDirection();

	// The distance between the center of the sphere and the nearest approach of the ray.
	double distanceNearestApproach = (nearestApproach - center).norm();

	// Nearest approach is farther than radius, so we never impacted. Attempting to compute will result in imaginary numbers
	if (distanceNearestApproach > radius)
		return std::nullopt;

	// See YouTube video in sources for more info
	double xVal = std::sqrt(std::pow(radius, 2) - std::pow(distanceNearestApproach, 2));

	// rayParamNearest +/- xVal denotes the two points that parametrically define the intersection of the ray and the sphere
	Eigen::Vector3d impactPosNear = ray.getPosition() + (rayParamNearest - xVal) * ray.getDirection();


	return impactPosNear;
}

Eigen::Vector3d Sphere::getNormalAt(Eigen::Vector3d position)
{
	return (position - center).normalized();
}

Eigen::Vector3d Sphere::getDiffuseCoefficient()
{
	return diffuseCoefficient;
}

Eigen::Vector3d Sphere::getSpecularCoefficient()
{
	return specularCoefficient;
}

double Sphere::getPhongExponent()
{
	return phongExponent;
}

Sphere::Sphere()
{

}

