#pragma once


#include "../scene/CameraMode.h"
#include "yaml-cpp/yaml.h"

class Options
{
public:
	[[nodiscard]] CameraMode getCameraMode() const;
	[[nodiscard]] const std::string& getSavePath() const;
	[[nodiscard]] int getRayBounces() const;

	static Options deserialize(const YAML::Node& node);

private:
	Options(CameraMode cameraMode, std::string savePath, int ttl);

	CameraMode cameraMode;
	std::string savePath;
	int rayBounces;
};
