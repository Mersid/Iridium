#include <iostream>
#include "Mesh.h"

Mesh::Mesh(std::vector<Triangle> triangles) : triangles(triangles)
{

}

std::vector<Triangle>& Mesh::getTriangles()
{
	return triangles;
}

std::vector<Eigen::Vector3d> Mesh::getVertices()
{
	std::vector<Eigen::Vector3d> vertices;
	vertices.reserve(3 * triangles.size());

	for (Triangle& triangle : triangles)
	{
		vertices.emplace_back(triangle.getA());
		vertices.emplace_back(triangle.getB());
		vertices.emplace_back(triangle.getC());
	}

	return vertices;
}

void Mesh::translate(Eigen::Vector3d translateBy)
{
	for (Triangle& t : triangles) // Forgetting the & cost me an hour :(  -- Steven, 2022-03-16
		t.move(translateBy);
}

