#pragma once

#include "Animation2D/Animation2D.h"
#include "Animation2D/Animation2DTransition.h"
#include <vector>

class Animation2DState
{
public:
	Animation2DState(const std::string & name, Animation2DRef animation, float animationSpeed = 1.0f, bool xFliped = false, bool yFliped = false);
	~Animation2DState() = default;
	Animation2DState(Animation2DState &&) = default;
	Animation2DState & operator=(Animation2DState &&) = default;

	inline Animation2DRef getAnimation() const { return m_animation; }

	inline void setSpeed(float value) { m_animationSpeed = value; }
	inline float getSpeed() const { return m_animationSpeed; }

	inline void setXFliped(bool value) { m_xFliped = value; }
	inline bool getXfliped() const { return m_xFliped; }

	inline void setYFliped(bool value) { m_yFliped = value; }
	inline bool getYFliped() const { return m_yFliped; }

	inline void setName(const std::string & name) { m_name = name; }
	inline std::string getName() const { return m_name; }

	inline void addTransition(Animation2DTransition && transition) { m_transitions.push_back(std::move(transition)); }
	inline Animation2DTransition & transition(size_t index) { return m_transitions[index]; }
	inline Animation2DTransition const & transition(size_t index) const { return m_transitions[index]; }

	inline size_t transitionCount() const { return m_transitions.size(); }

private:
	Animation2DRef m_animation;
	float m_animationSpeed;
	bool m_xFliped;
	bool m_yFliped;
	std::vector<Animation2DTransition> m_transitions;
	std::string m_name;
};

