#pragma once

#include "Animation2D/Animation2D.h"

class Animation2DState
{
public:
	Animation2DState(Animation2DRef animation, float animationSpeed = 1.0f, bool xFliped = false, bool yFliped = false);
	~Animation2DState() = default;

	inline Animation2DRef getAnimation() const { return m_animation; }

	inline void setSpeed(float value) { m_animationSpeed = value; }
	inline float getSpeed() const { return m_animationSpeed; }

	inline void setXFliped(bool value) { m_xFliped = value; }
	inline bool getXfliped() const { return m_xFliped; }

	inline void setYFliped(bool value) { m_yFliped = value; }
	inline bool getYFliped() const { return m_yFliped; }

private:
	Animation2DRef m_animation;
	float m_animationSpeed;
	bool m_xFliped;
	bool m_yFliped;
};

