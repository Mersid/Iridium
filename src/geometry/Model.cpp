#include "Model.h"

#include <utility>

Model::Model(Mesh mesh, const Material& material) : mesh(std::move(mesh)), bvh(nullptr)
{
	setMaterial(material);
}

void Model::setMaterial(const Material& material)
{
	for (Primitive* primitive : mesh.getPrimitives())
	{
		primitive->setMaterial(material);
	}
}

std::optional<RayTraceInfo> Model::intersect(const Ray& ray)
{
	Primitive* nearestObject = nullptr;
	Eigen::Vector3d nearestHitPos; // The hit pos of the nearest object. We need this to compare with the current object, and replace it if it's closer than this one

	std::vector<Primitive*> consideredPrimitives = bvh == nullptr ? mesh.getPrimitives() : getPossibleIntersects(ray);

	for (Primitive* primitive : consideredPrimitives)
	{
		std::optional<Eigen::Vector3d> possibleHit = primitive->getRayIntersection(ray);
		if (!possibleHit.has_value())
			continue; // We missed. Continue on with the next object

		// We have a value for possibleHit, so it did hit the primitive we're checking against.
		Eigen::Vector3d hitPos = possibleHit.value();

		// We hit something, and we haven't hit anything else yet for this ray, so set it
		// OR: If our new object has a shorter ray than the previous closest object, replace it.
		if (nearestObject == nullptr || (hitPos - ray.getPosition()).norm() < (nearestHitPos - ray.getPosition()).norm())
		{
			nearestObject = primitive;
			nearestHitPos = hitPos;
		}
	}

	if (nearestObject == nullptr)
		return std::nullopt;
	return RayTraceInfo(nearestObject, nearestHitPos);
}

void Model::translate(const Eigen::Vector3d& translateBy)
{
	mesh.translate(translateBy);
}

void Model::generateBVH()
{
	bvh = std::make_shared<BoundingVolumeHierarchy>(mesh.getPrimitives());
}

std::vector<Primitive*> Model::getPossibleIntersects(const Ray& ray)
{
	std::vector<Primitive*> bvhHits;
	BoundingVolumeHierarchy::intersectsBVH(bvh, ray, bvhHits);
	return bvhHits;
}
