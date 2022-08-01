#pragma once

#include "Eigen/Core"
#include "Eigen/Dense"
#include "Ray.h"
#include "Primitive.h"
#include "Mesh.h"
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
	 * @param mesh The mesh that this parallelogram belongs to
	 */
	Parallelogram(const Eigen::Vector3d& a, const Eigen::Vector3d& b, const Eigen::Vector3d& c, Mesh* mesh);

	/**
	 * For serialization purposes only
	 */
	Parallelogram() = default;

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

	Box getBoundingBox() override;

	Mesh* getMesh() override;

	void setMesh(Mesh* mesh) override;

	/**
	 * Transforms the parallelogram according to the given node
	 */
	void deserialize(const YAML::Node& node);

private:
	Eigen::Vector3d a; // Common point
	Eigen::Vector3d b; // Usually the horizontal point
	Eigen::Vector3d c; // Usually the vertical point

	Eigen::Vector3d getU(); // Vector ab, equal to b-a
	Eigen::Vector3d getV(); // Vector ac, equal to c-a
	Eigen::Vector3d getNormal(); // Normal vector of this triangle, equal to cross product of u x v.

	Mesh* mesh; // The mesh this primitive belongs to

};

template<>
struct YAML::convert<Parallelogram>
{
	static bool decode(const Node& node, Parallelogram& parallelogram)
	{
		parallelogram.deserialize(node);
	}
};
