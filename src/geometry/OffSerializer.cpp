#include <fstream>
#include "OffSerializer.h"
#include "../utils/StringUtils.h"
#include <memory>

std::unique_ptr<Mesh> OffSerializer::loadOff(const std::string& path)
{
	std::ifstream stream(path);
	std::stringstream ss;
	ss << stream.rdbuf();

	std::vector<std::string> lines = StringUtils::splitString(ss.str());
	std::vector<std::string> metadataLine = StringUtils::splitString(lines[1], ' ');

	unsigned long vertexCount = std::stoul(metadataLine[0]);
	unsigned long trisCount = std::stoul(metadataLine[1]);

	std::vector<Eigen::Vector3d> vertices;
	vertices.reserve(vertexCount);

	// Load vertices
	for (std::vector<std::string>::size_type i = 2; i < 2 + vertexCount; i++)
	{
		std::vector<std::string> tokens = StringUtils::splitString(lines[i], ' ');
		double x = std::stod(tokens[0]);
		double y = std::stod(tokens[1]);
		double z = std::stod(tokens[2]);
		vertices.emplace_back(x, y, z);
	}

	std::unique_ptr<Mesh> mesh;

	// Load triangles. Start offset is 2 (for magic number and metadata, one line each) + the number of vertices
	for (std::vector<std::string>::size_type i = 2 + vertexCount; i < 2 + vertexCount + trisCount; i++)
	{
		std::vector<std::string> tokens = StringUtils::splitString(lines[i], ' ');

		unsigned long indexA = std::stoul(tokens[1]);
		unsigned long indexB = std::stoul(tokens[2]);
		unsigned long indexC = std::stoul(tokens[3]);

		std::unique_ptr<Triangle> triangle = std::make_unique<Triangle>(vertices[indexA], vertices[indexB], vertices[indexC], &*mesh);
		mesh->addPrimitive(std::move(triangle));
	}

	return mesh;
}
