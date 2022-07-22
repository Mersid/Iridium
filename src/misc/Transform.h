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

	[[nodiscard]] const Eigen::Vector3d& getPosition() const;
	[[nodiscard]] const Eigen::Vector3d& getRotation() const;
	[[nodiscard]] const Eigen::Vector3d& getScale() const;
	void setPosition(const Eigen::Vector3d& position);
	void setRotation(const Eigen::Vector3d& rotation);
	void setScale(const Eigen::Vector3d& scale);

private:
	Eigen::Vector3d position;
	Eigen::Vector3d rotation;
	Eigen::Vector3d scale;
};
