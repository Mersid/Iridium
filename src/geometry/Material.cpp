#include "Material.h"
#include "../misc/Vector3dConvert.h"
#include <utility>

Material::Material(Eigen::Vector3d diffuseCoefficient, Eigen::Vector3d specularCoefficient, double phongExponent,
                   double reflectionCoefficient) :
				   diffuseCoefficient(std::move(diffuseCoefficient)), specularCoefficient(std::move(specularCoefficient)), phongExponent(phongExponent), reflectionCoefficient(reflectionCoefficient)
{
}

const Eigen::Vector3d& Material::getDiffuseCoefficient() const
{
	return diffuseCoefficient;
}

const Eigen::Vector3d& Material::getSpecularCoefficient() const
{
	return specularCoefficient;
}

double Material::getPhongExponent() const
{
	return phongExponent;
}

double Material::getReflectionCoefficient() const
{
	return reflectionCoefficient;
}

Material Material::deserialize(const YAML::Node& node)
{
	auto diffuseCoefficient = node["diffuseCoefficient"] ? node["diffuseCoefficient"].as<Eigen::Vector3d>() : Eigen::Vector3d(0.5, 0.5, 0.5);
	auto specularCoefficient = node["specularCoefficient"] ? node["specularCoefficient"].as<Eigen::Vector3d>() : Eigen::Vector3d(0.2, 0.2, 0.2);
	auto phongExponent = node["phongExponent"] ? node["phongExponent"].as<double>() : 256;
	auto reflectionCoefficient = node["reflectionCoefficient"] ? node["reflectionCoefficient"].as<double>() : 0;

	Material material(diffuseCoefficient, specularCoefficient, phongExponent, reflectionCoefficient);
	return material;
}
