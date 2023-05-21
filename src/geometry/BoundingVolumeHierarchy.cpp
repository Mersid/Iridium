#include "BoundingVolumeHierarchy.h"

BoundingVolumeHierarchy::BoundingVolumeHierarchy(std::vector<Primitive*> primitives, unsigned int depth) : primitive(nullptr)
{
	// Create bounding boxes for the BVH. Even leaf nodes get one, so it goes before the case check.
	std::vector<Box> boxes;
	boxes.reserve(primitives.size());
	for (Primitive* p : primitives)
		boxes.emplace_back(p->getBoundingBox());
	box = Box::aggregate(boxes);

	// Base case: The primitives vector has a single element, so that we have reached the end for this branch.
	if (primitives.size() == 1)
	{
		primitive = primitives[0];
		return;
	}

	// Sort the vector of primitives by x, y, or z, depending on the value of i.
	std::sort(primitives.begin(), primitives.end(), [depth](Primitive* a, Primitive* b){
		if (depth % 3 == 0)
			return a->getBoundingBox().getPosition().x() < b->getBoundingBox().getPosition().x();
		if (depth % 3 == 1)
			return a->getBoundingBox().getPosition().y() < b->getBoundingBox().getPosition().y();

		return a->getBoundingBox().getPosition().z() < b->getBoundingBox().getPosition().z();
	});

	// Split the primitives vector in half. While it's arguable easier to reason if for odd-sized vectors the middle element is on the left,
	// from a programming standpoint it's easier to leave it on the right. We can just do size / 2, which does floor division. The middle element
	// will end up on the right, but that's not a problem.

	std::vector<Primitive*> leftPrimitives(std::vector<Primitive*>(primitives.begin(), primitives.begin() + (primitives.size() / 2)));
	std::vector<Primitive*> rightPrimitives(std::vector<Primitive*>(primitives.begin() + (primitives.size() / 2), primitives.end()));

	left = std::make_unique<BoundingVolumeHierarchy>(leftPrimitives, depth + 1);
	right = std::make_unique<BoundingVolumeHierarchy>(rightPrimitives, depth + 1);

}


bool BoundingVolumeHierarchy::intersectsBVH(std::unique_ptr<BoundingVolumeHierarchy>& bvh, const Ray& ray,
                                            std::vector<Primitive*>& bvhHits)
{
	// Missed, can discard all below.
	if (!bvh->box.intersect(ray))
		return false;

	// Didn't miss.

	// Leaf node. Add its primitive to the list of hits and return.
	if (bvh->primitive != nullptr)
	{
		bvhHits.emplace_back(bvh->primitive);
		return true;
	}

	// Branch node. Recurse.
	intersectsBVH(bvh->left, ray, bvhHits);
	intersectsBVH(bvh->right, ray, bvhHits);

	return true;
}
