#pragma once


#include <optional>
#include "Eigen/Core"
#include "../texture/Texture.h"

class Material
{
public:
	/**
	 * @param diffuseCoefficient The diffuse coefficient for RGB
	 * @param specularCoefficient The specular coefficient for RGB
	 * @param phongExponent Phong exponent for specular shading. Larger number means smaller, brighter point
	 * @param reflectionCoefficient What ratio of light to reflect. 1 means it loses no energy on bounce
	 * @param texture Optional texture to apply to the object
	 */
	Material(Eigen::Vector3d diffuseCoefficient = Eigen::Vector3d(0.5, 0.5, 0.5),
			 Eigen::Vector3d specularCoefficient = Eigen::Vector3d(0.5, 0.5, 0.5),
			 double phongExponent = 10,
			 double reflectionCoefficient = 0,
			 std::optional<Texture> texture = std::nullopt);

	[[nodiscard]] const Eigen::Vector3d& getDiffuseCoefficient() const;

	[[nodiscard]] const Eigen::Vector3d& getSpecularCoefficient() const;

	[[nodiscard]] double getPhongExponent() const;

	[[nodiscard]] double getReflectionCoefficient() const;

	const std::optional<Texture>& getTexture() const;


private:
	Eigen::Vector3d diffuseCoefficient;
	Eigen::Vector3d specularCoefficient;
	double phongExponent; // From slide set 5: ~100: shiny, ~1000: glossy, ~10000: mirror
	double reflectionCoefficient;

	std::optional<Texture> texture;
};


