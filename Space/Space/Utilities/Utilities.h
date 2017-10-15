#pragma once

#include <Nazara/Math/Vector2.hpp>

inline float normalize(float value, float min, float max)
{
	return (value - min) / (max - min);
}

inline float lerp(float minValue, float maxValue, float t)
{
	return minValue * t + maxValue * (1 - t);
}

inline float sign(float value)
{
	return value >= 0 ? 1.0f : -1.0f;
}

inline float signedAngle(const Nz::Vector2f & vect)
{
	return atan2(vect.y, vect.x) * 180.0f / float(M_PI);
}
