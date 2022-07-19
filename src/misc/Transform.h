#pragma once


#include "Eigen/Core"
#include "../defs/TransformDef.h"

class Transform
{
public:
	Transform() = default;
	explicit Transform(const TransformDef& def);

	Transform(const Transform& other) = default;
	Transform(Transform&& other) = default;
	Transform& operator=(const Transform& other) = default;
	Transform& operator=(Transform&& other) = default;

private:
	Eigen::Vector3d position;
	Eigen::Vector3d rotation;
	Eigen::Vector3d scale;
};
