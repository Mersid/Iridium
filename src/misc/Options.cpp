#include <string>
#include "Options.h"

Options::Options(CameraMode cameraMode, std::string savePath, int rayBounces, bool ppm) :
	cameraMode(cameraMode), savePath(savePath), rayBounces(rayBounces), ppm(ppm)
{
}

CameraMode Options::getCameraMode() const
{
	return cameraMode;
}

const std::string& Options::getSavePath() const
{
	return savePath;
}

int Options::getRayBounces() const
{
	return rayBounces;
}

bool Options::isPpm() const {
    return ppm;
}

Options Options::deserialize(const YAML::Node& node)
{
	auto cameraModeString = node["cameraMode"].as<std::string>();
	auto savePath = node["savePath"].as<std::string>();
	auto rayBounces = node["rayBounces"].as<int>();

	CameraMode cameraMode = cameraModeString == "orthographic" ? CameraMode::ORTHOGRAPHIC : CameraMode::PERSPECTIVE;

    auto ppm = node["outputFormat"].as<std::string>() == "ppm";

	Options options(cameraMode, savePath, rayBounces, ppm);
	return options;
}
