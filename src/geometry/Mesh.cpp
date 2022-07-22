#include <iostream>
#include <utility>
#include "Mesh.h"
#include "Primitive.h"

Mesh::Mesh(std::vector<std::shared_ptr<Primitive>> primitives) : primitives(std::move(primitives))
{

}

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
