#include "Animation2DState.h"

Animation2DState::Animation2DState(Animation2DRef animation, float animationSpeed, bool xFliped, bool yFliped)
	: m_animation(animation)
	, m_animationSpeed(animationSpeed)
	, m_xFliped(xFliped)
	, m_yFliped(yFliped)
{

}

