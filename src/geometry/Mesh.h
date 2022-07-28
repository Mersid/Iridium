#pragma once

#include "Primitive.h"

/**
 * A mesh is a collection of primitives
 */
class Mesh
{
public:
	// TODO: Maybe unique_ptr?
	void addPrimitive(std::shared_ptr<Primitive>&& primitive);

	std::vector<Primitive*> getPrimitives();

	Material& getMaterial();

	void setMaterial(const Material& material);

private:
	std::vector<std::shared_ptr<Primitive>> primitives;
	Material material;
};
