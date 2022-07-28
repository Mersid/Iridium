#include <iostream>
#include <utility>
#include "Mesh.h"
#include "Primitive.h"

std::vector<Primitive*> Mesh::getPrimitives()
{
	std::vector<Primitive*> ptrs;
	ptrs.reserve(primitives.size());
	for (const std::shared_ptr<Primitive>& primitive : primitives)
		ptrs.emplace_back(primitive.get());
	return ptrs;
}

void Mesh::addPrimitive(std::shared_ptr<Primitive>&& primitive)
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
