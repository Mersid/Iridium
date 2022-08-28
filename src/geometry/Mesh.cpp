#include <iostream>
#include <utility>
#include "Mesh.h"
#include "Primitive.h"
#include "../Shimmerlight.h"

std::vector<Primitive*> Mesh::getPrimitives()
{
	std::vector<Primitive*> ptrs;
	ptrs.reserve(primitives.size());
	for (const std::shared_ptr<Primitive>& primitive : primitives)
		ptrs.emplace_back(primitive.get());
	return ptrs;
}

void Mesh::addPrimitive(std::shared_ptr<Primitive> primitive)
{
	primitives.emplace_back(primitive);
}

Material& Mesh::getMaterial()
{
	return material;
}

void Mesh::setMaterial(const Material& material)
{
	Mesh::material = material;
}

Mesh Mesh::deserialize(const YAML::Node& node)
{
	std::shared_ptr<Primitive> primitive;
	Mesh mesh;
	Material material = Material::deserialize(node["material"]);

	if (node["parallelogram"])
		primitive = Parallelogram::deserialize(node["parallelogram"]);
	else if (node["sphere"])
		primitive = Sphere::deserialize(node["sphere"]);
	else if (node["triangle"])
		primitive = Triangle::deserialize(node["triangle"]);

	if (primitive)
	{
		mesh.addPrimitive(primitive);
		mesh.setMaterial(material);
		return mesh;
	}

	// Complex mesh
	mesh = Shimmerlight::getInstance()->getOffSerializer().loadOff(node["file"]["path"].as<std::string>());
	mesh.setMaterial(material);
	return mesh;
}

void Mesh::setPrimitivesOwner()
{
	for (std::shared_ptr<Primitive>& primitive : primitives)
		primitive->setMesh(this);
}
