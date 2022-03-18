#include "RayTraceInfo.h"

#include <utility>

RayTraceInfo::RayTraceInfo(std::shared_ptr<Primitive> primitive, Eigen::Vector3d hitPos) : hitPrimitive(std::move(primitive)), hitPos(std::move(hitPos))
{

}

std::shared_ptr<Primitive> RayTraceInfo::getHitPrimitive() const
{
	return hitPrimitive;
}

const Eigen::Vector3d& RayTraceInfo::getHitPos() const
{
	return hitPos;
}
