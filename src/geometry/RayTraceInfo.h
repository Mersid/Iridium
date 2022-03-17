#pragma once


#include "Primitive.h"

class RayTraceInfo
{
public:
	RayTraceInfo(Primitive* primitive, Eigen::Vector3d hitPos);

	[[nodiscard]] Primitive* getHitPrimitive() const;

	[[nodiscard]] const Eigen::Vector3d& getHitPos() const;

private:
	Primitive* hitPrimitive;
	Eigen::Vector3d hitPos;
};


