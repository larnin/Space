#include "Animator2D.h"
#include <algorithm>

Animator2D::Animator2D(Animation2DRef animation)
{
	addAnimation(animation);
	addState(Animation2DState(animation->getName(), animation));
}

void Animator2D::addAnimation(Animation2DRef animation)
{
	m_animations.push_back(animation);
}

void Animator2D::removeAnimation(Animation2DRef animation)
{
	m_animations.erase(std::remove(m_animations.begin(), m_animations.end(), animation), m_animations.end());
	m_states.erase(std::remove_if(m_states.begin(), m_states.end(), [animation](const auto & e) {return e.getAnimation() == animation; }), m_states.end());
}

void Animator2D::removeAnimation(const std::string & name)
{
	if (!animationExist(name))
		return;
	removeAnimation(getAnimation(name));
}

Animation2DRef Animator2D::getAnimation(const std::string & name) const
{
	for (const auto & anim : m_animations)
		if (anim->getName() == name)
			return anim;
	return Animation2DRef();
}

bool Animator2D::animationExist(const std::string & name) const
{
	for (const auto & anim : m_animations)
		if (anim->getName() == name)
			return true;
	return false;
}

size_t Animator2D::getAnimationCount() const
{
	return m_animations.size();
}

Animation2DRef Animator2D::getAnimation(size_t index) const
{
	return m_animations[index];
}

Animation2DState & Animator2D::addState(Animation2DState && state)
{
	NazaraAssert(std::find(m_animations.begin(), m_animations.end(), state.getAnimation()) != m_animations.end(), "You must add the animation on the animator before the state !");
	m_states.push_back(std::move(state));
	return m_states.back();
}

void Animator2D::removeState(const Animation2DState & state)
{
	m_states.erase(std::remove_if(m_states.begin(), m_states.end(), [&state](const auto & e) {return &e == &state; }), m_states.end());
}

void Animator2D::removeState(const std::string & name)
{
	m_states.erase(std::remove_if(m_states.begin(), m_states.end(), [&name](const auto & e) {return name == e.getName(); }), m_states.end());
}

Animation2DState const & Animator2D::getState(const std::string & name) const
{
	for (auto & s : m_states)
		if (s.getName() == name)
			return s;
	NazaraAssert(false, "The state can't be found !");
	return m_states.front();
}

Animation2DState & Animator2D::getState(const std::string & name)
{
	for (auto & s : m_states)
		if (s.getName() == name)
			return s;
	NazaraAssert(false, "The state can't be found !");
	return m_states.front();
}

bool Animator2D::stateExist(const std::string & name) const
{
	for (const auto & s : m_states)
		if (s.getName() == name)
			return true;
	return false;
}

void Animator2D::setDefaultStateName(const std::string & name)
{
	m_defaultStateName = name;
}

std::string Animator2D::getDefaultStateName() const
{
	return m_defaultStateName;
}

