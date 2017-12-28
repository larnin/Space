#pragma once

#include <Nazara/Utility/Image.hpp>
#include <Nazara/Math/Vector2.hpp>
#include "AsteroidBaseInfos.h"

struct AsteroidExplosion
{
	inline AsteroidExplosion(float _radius, const Nz::Vector2i & _localPos)
		: radius(_radius), localPos(_localPos) {}
	float radius;
	Nz::Vector2i localPos;
};

AsteroidData createAsteroidData(const Nz::Image & asteroid);
void createRender(Nz::Image & target, const Nz::Image & asteroid, const Nz::Image & explosion, const Nz::Image & border, const AsteroidData & data, const std::vector<AsteroidExplosion> & explosions);