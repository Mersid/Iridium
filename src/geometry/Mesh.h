#pragma once


#include "Triangle.h"

/**
 * A mesh is a collection of primitives
 */
class Mesh
{
public:
	explicit Mesh(std::vector<std::shared_ptr<Primitive>> primitives);

	void addPrimitive(std::shared_ptr<Primitive>&& primitive);

	std::vector<Primitive*> getPrimitives();

private:
	std::vector<std::shared_ptr<Primitive>> primitives;
	Material material;
};
