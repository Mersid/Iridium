#pragma once


#include "Primitive.h"

/**
 * Represents a triangle. Note that the code is essentially copied from the parallelogram,
 * as their code is pretty much identical (a parallelogram is two triangles side by side)
 */
class Triangle : public Primitive
{
public:
	/**
	 * @param a The base point of the triangle
	 * @param b The second point of the triangle. Consider the cross product will use the vector ab as its A value (for A X B)
	 * @param c The third point of the triangle. Consider the cross product will use the vector ac as its B value (for A X B)
	 * @param material The properties of the material of this primitive
	 */
	Triangle(Eigen::Vector3d a, Eigen::Vector3d b, Eigen::Vector3d c, Material material = Material());

	std::optional<Eigen::Vector3d> getRayIntersection(const Ray& ray) override;

	Eigen::Vector3d getNormalAt(Eigen::Vector3d position) override;

	Material& getMaterial() override;

	void setMaterial(Material mat) override;

	void move(Eigen::Vector3d newPos) override;

private:
	Eigen::Vector3d a; // Common point
	Eigen::Vector3d b; // Usually the horizontal point
	Eigen::Vector3d c; // Usually the vertical point

	Eigen::Vector3d u; // Vector ab, equal to b-a
	Eigen::Vector3d v; // Vector ac, equal to c-a
	Eigen::Vector3d normal; // Normal vector of this triangle, equal to cross product of u x v.

	Material material;

};


