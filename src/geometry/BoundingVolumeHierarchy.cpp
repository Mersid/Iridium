#include "BoundingVolumeHierarchy.h"

BoundingVolumeHierarchy::BoundingVolumeHierarchy(std::vector<Primitive*> primitives, unsigned int i)
{

	// Base case: The primitives vector has a single element, so that we have reached the end for this branch.
	if (primitives.size() == 1)
	{
		primitive = primitives[0];
		return;
	}

	// Sort the vector of primitives by x, y, or z, depending on the value of i.
	std::sort(primitives.begin(), primitives.end(), [i](Primitive* a, Primitive* b){
		if (i % 3 == 0)
			return a->getPosition().x() < b->getPosition().x();
		if (i % 3 == 1)
			return a->getPosition().y() < b->getPosition().y();

		return a->getPosition().z() < b->getPosition().z();
	});

	// Split the primitives vector in half. While it's arguable easier to reason if for odd-sized vectors the middle element is on the left,
	// from a programming standpoint it's easier to leave it on the right. We can just do size / 2, which does floor division. The middle element
	// will end up on the right, but that's not a problem.

	std::vector<Primitive*> leftPrimitives(std::vector<Primitive*>(primitives.begin(), primitives.begin() + (primitives.size() / 2)));
	std::vector<Primitive*> rightPrimitives(std::vector<Primitive*>(primitives.begin() + (primitives.size() / 2), primitives.end()));

	left = std::make_shared<BoundingVolumeHierarchy>(leftPrimitives, i + 1);
	right = std::make_shared<BoundingVolumeHierarchy>(rightPrimitives, i + 1);

}
