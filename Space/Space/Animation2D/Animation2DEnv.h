#pragma once

#include "Animation2D/Animation2DPropertiesHolder.h"

struct Animation2DEnv
{
	Animation2DEnv(float _animationTime, bool _animationFinished, Animation2DPropertiesHolder _propertiesHolder)
		: animationTime(_animationTime)
		, animationsFinished(_animationFinished)
		, propertyHolder(_propertiesHolder)
	{
		
	}

	float animationTime;
	bool animationsFinished;
	const Animation2DPropertiesHolder & propertyHolder;
};

