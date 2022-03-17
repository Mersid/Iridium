#include "RayTraceInfo.h"

RayTraceInfo::RayTraceInfo(Primitive* primitive, const Eigen::Vector3d& hitPos) : hitPrimitive(primitive), hitPos(hitPos)
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
