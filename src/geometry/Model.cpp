#include "Model.h"

#include <utility>
#include "Eigen/Dense"

Model::Model(Mesh mesh, const Material& material, bool applyMaterial) : mesh(std::move(mesh)), bvh(nullptr),
	position(Eigen::Vector3d::Zero()), rotation(Eigen::Vector3d::Zero()), scale(Eigen::Vector3d::Ones())
{
	if (applyMaterial)
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

void Model::setPosition(const Eigen::Vector3d& position)
{
	Model::position = position;
}

void Model::setRotation(const Eigen::Vector3d& rotation)
{
	Model::rotation = rotation;
}

void Model::setScale(const Eigen::Vector3d& scale)
{
	Model::scale = scale;
}

void Model::applyTransforms()
{
	Eigen::Matrix4d scalingMatrix;
	scalingMatrix <<
		scale.x(), 0, 0, 0,
		0, scale.y(), 0, 0,
		0, 0, scale.z(), 0,
		0, 0, 0, 1;

	Eigen::Matrix4d translationMatrix;
	translationMatrix <<
		1, 0, 0, position.x(),
		0, 1, 0, position.y(),
		0, 0, 1, position.z(),
		0, 0, 0, 1;

	double xRad = rotation.x() * EIGEN_PI / 180;
	double yRad = rotation.y() * EIGEN_PI / 180;
	double zRad = rotation.z() * EIGEN_PI / 180;

	Eigen::Matrix4d rotationXMatrix;
	rotationXMatrix <<
		1, 0, 0, 0,
		0, std::cos(xRad), -std::sin(xRad), 0,
		0, std::sin(xRad), cos(xRad), 0,
		0, 0, 0, 1;

	Eigen::Matrix4d rotationYMatrix;
	rotationYMatrix <<
		std::cos(yRad), 0, std::sin(yRad), 0,
		0, 1, 0, 0,
		-std::sin(yRad), 0, std::cos(yRad), 0,
		0, 0, 0, 1;

	Eigen::Matrix4d rotationZMatrix;
	rotationZMatrix <<
		std::cos(zRad), -std::sin(zRad), 0, 0,
		std::sin(zRad), std::cos(zRad), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1;

	Eigen::Matrix4d rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;

	Eigen::Matrix4d combinedMatrix = translationMatrix * rotationMatrix * scalingMatrix;

	for (Primitive* p : mesh.getPrimitives())
	{
		for (Eigen::Vector3d* vertex : p->getVertices())
		{
			*vertex = (combinedMatrix * vertex->homogeneous()).head<3>();
		}
	}
}
