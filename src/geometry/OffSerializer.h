#pragma once


#include <vector>
#include "Triangle.h"

/**
 * Limited-functionality .OFF file loader. Assumes the second line has the vertex/face count, and that each face is a triangle
 * (first one is always 3 for the number of vertices). Colored faces are not supported (nor are most features, to be honest)
 * This implementation does not support comments.
 */
class OffSerializer
{
public:
	std::vector<std::shared_ptr<Triangle>> loadOff(const std::string& path);

private:

};


