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
	 * @param depth Incremented with depth. This is used with modulo to determine to split in x, y, or z axis (%0 = x, %1 = y, %2 = z). Root has depth 0
	 */
	explicit BoundingVolumeHierarchy(std::vector<Primitive*> primitives, unsigned int depth = 0);
	BoundingVolumeHierarchy() = default;

	/**
	 * Static recursive method call that gets a list of possible hits.
	 * @param bvh The BVH to test against.
	 * @param ray The ray to fire against the BVH and all its substructures
	 * @param bvhHits Pass in a reference to a vector, and the call will add on any possible hits to it
	 * @return True if it hit the main box, false otherwise
	 */
	static bool intersectsBVH(std::unique_ptr<BoundingVolumeHierarchy>& bvh, const Ray& ray, std::vector<Primitive*>& bvhHits);

private:
	std::unique_ptr<BoundingVolumeHierarchy> left;
	std::unique_ptr<BoundingVolumeHierarchy> right;
	Box box;
	Primitive* primitive;
};

