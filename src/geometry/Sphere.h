#pragma once

#include "Primitive.h"
#include "Material.h"

class Sphere : public Primitive
{
public:
	/**
	 * Creates a sphere
	 * @param center The 3d coordinates of where the center of the sphere is
	 * @param radius The radius of the sphere, relative to the center
	 * @param mesh The mesh that this sphere belongs to
	 */
	Sphere(Eigen::Vector3d center, double radius, Mesh& mesh);

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

	Box getBoundingBox() override;

	Mesh& getMesh() override;

private:
	Eigen::Vector3d center;
	double radius{};
	Mesh& mesh;
};

