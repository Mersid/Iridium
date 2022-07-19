#pragma once
#include "yaml-cpp/yaml.h"

struct CameraDef
{
	// The width of the camera, in pixels
	int width;
	// The height of the camera, in pixels
	int height;
	// Distance from the virtual plane to the aperture. Defined this way to keep focalLength positive
	double focalLength;
	// Radius of the aperture. Larger values may induce more depth of field blur. Use 0 for the default pinhole camera
	double apertureRadius;
	//The number of random rays to fire for the depth of field calculation. Use 1 if the aperture is 0 for default model
	int rayShots;
};

template<>
struct YAML::convert<CameraDef>
{
	static bool decode(const Node& node, CameraDef& def)
	{
		def.width = node["width"].as<int>();
		def.height = node["height"].as<int>();
		def.focalLength = node["focalLength"] ? node["focalLength"].as<double>() : 1;
		def.apertureRadius = node["apertureRadius"] ? node["apertureRadius"].as<double>() : 0;
		def.apertureRadius = node["rayShots"] ? node["rayShots"].as<int>() : 1;
	}
};
