#include "Transform.h"

Transform::Transform(const TransformDef& def) : position(def.position), rotation(def.rotation), scale(def.scale)
{
}

const Eigen::Vector3d& Transform::getPosition() const
{
	return position;
}

void Transform::setPosition(const Eigen::Vector3d& position)
{
	Transform::position = position;
}

const Eigen::Vector3d& Transform::getRotation() const
{
	return rotation;
}

void Transform::setRotation(const Eigen::Vector3d& rotation)
{
	Transform::rotation = rotation;
}

const Eigen::Vector3d& Transform::getScale() const
{
	return scale;
}

void Transform::setScale(const Eigen::Vector3d& scale)
{
	Transform::scale = scale;
}

Transform Transform::deserialize(const YAML::Node& node)
{
	Transform transform;
	transform.setPosition(node["position"].as<Eigen::Vector3d>());
	transform.setRotation(node["rotation"].as<Eigen::Vector3d>());
	transform.setScale(node["scale"].as<Eigen::Vector3d>());

	return transform;
}
