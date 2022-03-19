#pragma once

#include <memory>
#include "Box.h"
#include "Primitive.h"

class BoundingVolumeHierarchy
{
public:
	/**
	 * Recursively builds a BVH tree. Note that a tree contains subtrees, so this recursively builds the tree up.
	 * @param primitives The primitives to create a new tree from. It is illegal and undefined behavior for the vector to be empty.
	 * @param i Incremented with depth. This is used with modulo to determine to split in x, y, or z axis (%0 = x, %1 = y, %2 = z)
	 */
	explicit BoundingVolumeHierarchy(std::vector<Primitive*> primitives, unsigned int i = 0);
	BoundingVolumeHierarchy() = default;

	/**
	 * Static recursive method call that gets a list of possible hits. TODO: Consider making non-static
	 * @param bvh The BVH to test against.
	 * @param ray The ray to fire against the BVH and all its substructures
	 * @param bvhHits Pass in a reference to a vector, and the call will add on any possible hits to it
	 * @return True if it hit the main box, false otherwise
	 */
	static bool intersectsBVH(std::shared_ptr<BoundingVolumeHierarchy>& bvh, const Ray& ray, std::vector<Primitive*>& bvhHits);

private:
	std::shared_ptr<BoundingVolumeHierarchy> left;
	std::shared_ptr<BoundingVolumeHierarchy> right;
	Box box;
	Primitive* primitive;
};

