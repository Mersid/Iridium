#pragma once

#include "Primitive.h"
#include "yaml-cpp/yaml.h"
#include "Parallelogram.h"
#include "Sphere.h"
#include "Triangle.h"


/**
 * A mesh is a collection of primitives
 */
class Mesh
{
public:
	void addPrimitive(std::unique_ptr<Primitive> primitive);

	std::vector<Primitive*> getPrimitives();

	Material& getMaterial();

	void setMaterial(const Material& material);

	static std::unique_ptr<Mesh> deserialize(const YAML::Node& node);

private:
	Mesh() = default;

	std::vector<std::unique_ptr<Primitive>> primitives;
	Material material;
};
