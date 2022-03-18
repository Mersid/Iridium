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

private:
	std::shared_ptr<BoundingVolumeHierarchy> left;
	std::shared_ptr<BoundingVolumeHierarchy> right;
	Box box;
	Primitive* primitive;
};


