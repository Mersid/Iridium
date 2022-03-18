#include "RayTraceInfo.h"

#include <utility>

RayTraceInfo::RayTraceInfo(Primitive* primitive, Eigen::Vector3d hitPos) : hitPrimitive(primitive), hitPos(std::move(hitPos))
{

}

Primitive* RayTraceInfo::getHitPrimitive() const
{
	return hitPrimitive;
}

const Eigen::Vector3d& RayTraceInfo::getHitPos() const
{
	return hitPos;
}
