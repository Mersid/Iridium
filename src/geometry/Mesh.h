#pragma once


#include "Triangle.h"

/**
 * A mesh is a collection of primitives
 */
class Mesh
{
public:
	explicit Mesh(std::vector<std::shared_ptr<Primitive>> primitives);

	std::vector<std::shared_ptr<Primitive>>& getPrimitives();

	/**
	 * @param translateBy Translates the model by this amount
	 */
	void translate(const Eigen::Vector3d& translateBy);


private:
	std::vector<std::shared_ptr<Primitive>> primitives;
};


