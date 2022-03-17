#pragma once


#include "Triangle.h"

/**
 * A mesh is a collection of triangles. Other primitives are not currently supported to simplify things.
 */
class Mesh
{
public:
	explicit Mesh(std::vector<Triangle> triangles);

	std::vector<Triangle>& getTriangles();

	/**
	 * @return All the vertices that make up the mesh, in no particular order.
	 */
	std::vector<Eigen::Vector3d> getVertices();

	/**
	 * @param translateBy Translates the model by this amount
	 */
	void translate(Eigen::Vector3d translateBy);


private:
	std::vector<Triangle> triangles;
};


