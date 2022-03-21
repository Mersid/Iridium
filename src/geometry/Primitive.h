#pragma once


#include <optional>
#include "Eigen/Core"
#include "Ray.h"
#include "Material.h"
#include "Box.h"
#include <memory>

class Primitive
{
public:

	virtual ~Primitive() = default;

	/**
	 * Test if the given ray intersects this parallelogram
	 * @param ray The ray to test against
	 * @return The vector representing the position of the intersection with the given ray if the ray hit the parallelogram
	 */
	virtual std::optional<Eigen::Vector3d> getRayIntersection(const Ray& ray) = 0;

	/**
	 * We need primitives to keep track of a position for BVH purposes. It doesn't really matter too much how it's done exactly.
	 * @return The position of the primitive. It should probably correspond to one of its vertices
	 */
	virtual Eigen::Vector3d getPosition() = 0;

	/**
	 * @return A bounding box large enough to contain the given primitive
	 */
	virtual Box getBoundingBox() = 0;

	/**
	 * Gets the vector normal to the surface of the primitive at the specified position. The position should be close to the surface, or undefined
	 * behaviour may occur
	 * @param position The position to find the normal of
	 * @return A unit vector normal to the surface of the primitive at the specified position
	 */
	virtual Eigen::Vector3d getNormalAt(Eigen::Vector3d position) = 0;

	virtual void setMaterial(Material material) = 0;
	virtual Material& getMaterial() = 0;

	/**
	 * @param newPos Moves the primitive relative to its current position by this quantity
	 */
	virtual void translate(Eigen::Vector3d newPos) = 0;

	/**
	 * @return A vector of vertex coordinates of the primitive. This is useful for applying linear transformations against.
	 */
	virtual std::vector<Eigen::Vector3d*> getVertices() = 0;

	/**
	 * @return A copy of this object, with its own memory and stuff.
	 */
	virtual std::shared_ptr<Primitive> clone() = 0;
};


