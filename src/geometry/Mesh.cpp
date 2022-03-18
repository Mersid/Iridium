#include <iostream>
#include <utility>
#include "Mesh.h"

Mesh::Mesh(std::vector<std::shared_ptr<Primitive>> primitives) : primitives(std::move(primitives))
{

}

std::vector<std::shared_ptr<Primitive>>& Mesh::getPrimitives()
{
	return primitives;
}


void Mesh::translate(const Eigen::Vector3d& translateBy)
{
	for (const std::shared_ptr<Primitive>& primitive : primitives) // Forgetting the & cost me an hour :(  -- Steven, 2022-03-16
		primitive->translate(translateBy);
}

