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
	// TODO: Maybe unique_ptr?
	void addPrimitive(std::shared_ptr<Primitive> primitive);

	std::vector<Primitive*> getPrimitives();

	Material& getMaterial();

	void setMaterial(const Material& material);

	static Mesh deserialize(const YAML::Node& node);

private:
	std::vector<std::shared_ptr<Primitive>> primitives;
	Material material;
};
