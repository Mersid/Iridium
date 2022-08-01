#pragma once


#include "Primitive.h"
#include "Eigen/Dense"
#include "yaml-cpp/yaml.h"

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
	 * @param mesh The mesh that this triangle belongs to
	 */
	Triangle(const Eigen::Vector3d& a, const Eigen::Vector3d& b, const Eigen::Vector3d& c, Mesh* mesh);

	std::optional<Eigen::Vector3d> getRayIntersection(const Ray& ray) override;

	Eigen::Vector3d getNormalAt(Eigen::Vector3d position) override;

	Box getBoundingBox() override;

	[[nodiscard]] const Eigen::Vector3d& getA() const;

	[[nodiscard]] const Eigen::Vector3d& getB() const;

	[[nodiscard]] const Eigen::Vector3d& getC() const;

	Mesh* getMesh() override;
	void setMesh(Mesh* mesh) override;

	void deserialize(const YAML::Node& node);

private:
	Eigen::Vector3d a; // Common point
	Eigen::Vector3d b; // Usually the horizontal point
	Eigen::Vector3d c; // Usually the vertical point

	Eigen::Vector3d getU(); // Vector ab, equal to b-a
	Eigen::Vector3d getV(); // Vector ac, equal to c-a
	Eigen::Vector3d getNormal(); // Normal vector of this triangle, equal to cross product of u x v.

	Mesh* mesh;
};

template<>
struct YAML::convert<Triangle>
{
	static bool decode(const Node& node, Triangle& triangle)
	{
		triangle.deserialize(node);
	}
};
