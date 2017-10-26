#pragma once

#include "Animation2D/PropertiesHolder.h"

struct Animation2DEnv
{
	Animation2DEnv(float _animationTime, bool _animationFinished, const PropertiesHolder & _propertiesHolder)
		: animationTime(_animationTime)
		, animationsFinished(_animationFinished)
		, propertyHolder(_propertiesHolder)
	{
		
	}

	float animationTime;
	bool animationsFinished;
	const PropertiesHolder & propertyHolder;
};

