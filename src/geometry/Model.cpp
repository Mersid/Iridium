#include "Model.h"

#include <utility>
#include "Eigen/Dense"

Model::Model(std::unique_ptr<Mesh> mesh) : mesh(std::move(mesh)), bvh(nullptr),
	transform(Transform())
{

}

std::optional<RayTraceInfo> Model::intersect(const Ray& ray)
{
	Primitive* nearestObject = nullptr;
	Eigen::Vector3d nearestHitPos; // The hit pos of the nearest object. We need this to compare with the current object, and replace it if it's closer than this one

	std::vector<Primitive*> consideredPrimitives = bvh == nullptr ? mesh->getPrimitives() : getPossibleIntersects(ray);

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

void Model::generateBVH()
{
	bvh = std::make_unique<BoundingVolumeHierarchy>(mesh->getPrimitives());
}

std::vector<Primitive*> Model::getPossibleIntersects(const Ray& ray)
{
	std::vector<Primitive*> bvhHits;
	BoundingVolumeHierarchy::intersectsBVH(bvh, ray, bvhHits);
	return bvhHits;
}

Transform& Model::getTransform()
{
	return transform;
}

std::unique_ptr<Model> Model::deserialize(const YAML::Node& node)
{
	std::unique_ptr<Mesh> mesh = Mesh::deserialize(node["mesh"]);
	Transform transform = Transform::deserialize(node["transform"]);
	std::unique_ptr<Model> model = instantiate(std::move(mesh));
	model->transform = transform;

	return model;
}

std::unique_ptr<Model> Model::instantiate(std::unique_ptr<Mesh> mesh)
{
	return std::unique_ptr<Model>(new Model(std::move(mesh)));
}

void Model::setMesh(std::unique_ptr<Mesh> mesh)
{
	this->mesh = std::move(mesh);
}

void Model::applyTransform() {
    // https://stackoverflow.com/questions/51315825/precedence-in-eigen-transformations-and-difference-between-pretranslate-and-tran
    Eigen::Transform<double, 3, Eigen::Affine> t;
    t = Eigen::Scaling(transform.getScale());
    t.prerotate(Eigen::AngleAxis<double>(transform.getRotation().x() * EIGEN_PI / 180, Eigen::Vector3d::UnitX()));
    t.prerotate(Eigen::AngleAxis<double>(transform.getRotation().y() * EIGEN_PI / 180, Eigen::Vector3d::UnitY()));
    t.prerotate(Eigen::AngleAxis<double>(transform.getRotation().z() * EIGEN_PI / 180, Eigen::Vector3d::UnitZ()));
    t.pretranslate(transform.getPosition());

    Eigen::Matrix4d m = t.matrix();

    mesh->applyTransformationMatrix(m);
}
