#include "Model.h"

Model::Model(const Mesh& mesh, const Material& material) : mesh(mesh)
{
	setMaterial(material);
	computeBoundingBox();
}

void Model::setMaterial(const Material& material)
{
	for (Triangle& triangle : mesh.getTriangles())
	{
		triangle.setMaterial(material);
	}
}

void Model::computeBoundingBox()
{
	double dbMax = std::numeric_limits<double>::max();
	double dbMin = std::numeric_limits<double>::min();
	Eigen::Vector3d min(dbMax, dbMax, dbMax);
	Eigen::Vector3d max(dbMin, dbMin, dbMin);

	for (Eigen::Vector3d vertex : mesh.getVertices())
	{
		min[0] = std::min(min[0], vertex.x());
		min[1] = std::min(min[1], vertex.y());
		min[2] = std::min(min[2], vertex.z());

		max[0] = std::max(max[0], vertex.x());
		max[1] = std::max(max[1], vertex.y());
		max[2] = std::max(max[2], vertex.z());
	}

	boundingBox = Box(min, max);
}

Box& Model::getBoundingBox()
{
	return boundingBox;
}

std::optional<RayTraceInfo> Model::intersect(const Ray& ray)
{
	Primitive* nearestObject = nullptr;
	Eigen::Vector3d nearestHitPos; // The hit pos of the nearest object. We need this to compare with the current object, and replace it if it's closer than this one

	for (Triangle& triangle : mesh.getTriangles())
	{
		std::optional<Eigen::Vector3d> possibleHit = triangle.getRayIntersection(ray);
		if (!possibleHit.has_value())
			continue; // We missed. Continue on with the next object

		// We have a value for possibleHit, so it did hit the primitive we're checking against.
		Eigen::Vector3d hitPos = possibleHit.value();

		// We hit something, and we haven't hit anything else yet for this ray, so set it
		// OR: If our new object has a shorter ray than the previous closest object, replace it.
		if (nearestObject == nullptr || (hitPos - ray.getPosition()).norm() < (nearestHitPos - ray.getPosition()).norm())
		{
			nearestObject = &triangle;
			nearestHitPos = hitPos;
		}
	}

	if (nearestObject == nullptr)
		return std::nullopt;
	return RayTraceInfo(nearestObject, nearestHitPos);
}

void Model::translate(Eigen::Vector3d translateBy)
{
	mesh.translate(translateBy);
}
