#pragma once
#include <NDK/Entity.hpp>
#include <NDK/World.hpp>
#include <Nazara/Math/Vector2.hpp>

struct ShipInfos
{
	std::string imageName; 
	float solidDrag;
	float fluidDrag;
	float acceleration;
	float maxRotationSpeed;
	float rotationAcceleration;
};

Ndk::EntityHandle createShip(Ndk::World & w, const ShipInfos & infos, const Nz::Vector2f & pos, float rotation);