#pragma once


#include "Primitive.h"

class RayTraceInfo
{
public:
	RayTraceInfo(std::shared_ptr<Primitive> primitive, Eigen::Vector3d hitPos);

	[[nodiscard]] std::shared_ptr<Primitive> getHitPrimitive() const;

	[[nodiscard]] const Eigen::Vector3d& getHitPos() const;

private:
	std::shared_ptr<Primitive> hitPrimitive;
	Eigen::Vector3d hitPos;
};


