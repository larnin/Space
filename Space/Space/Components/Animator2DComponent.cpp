#include "Animator2DComponent.h"

Animator2DComponent::Animator2DComponent(Animator2DRef animator, float animationSpeed)
	: m_animator(animator)
	, m_currentStateTime(0)
	,m_currentFrameTime(0)
	, m_animationSpeed(animationSpeed)
	, m_paused(false)
{
	resetAnimator();
}

void Animator2DComponent::resetAnimator()
{
	if (m_animator->stateExist(m_animator->getDefaultStateName()))
		m_currentState = &m_animator->getState(m_animator->getDefaultStateName());
}

bool Animator2DComponent::update(float elapsedTime)
{
	return false;
}

void Animator2DComponent::setAnimationSpeed(float value)
{
}

float Animator2DComponent::getAnimationSpeed() const
{
	return 0.0f;
}

const Frame & Animator2DComponent::getCurrentFrame() const
{
	// TODO: insérer une instruction return ici
}

void Animator2DComponent::pause()
{
}

void Animator2DComponent::start()
{
}

void Animator2DComponent::stop()
{
}

bool Animator2DComponent::isPaused() const
{
	return false;
}

void Animator2DComponent::attach(Nz::SpriteRef sprite)
{
}

void Animator2DComponent::detach(Nz::SpriteRef sprite)
{
}

void Animator2DComponent::detachAll()
{
}

void Animator2DComponent::setProperty(const std::string & key, int value)
{
}

int Animator2DComponent::getProperty(const std::string & key) const
{
	return 0;
}

bool Animator2DComponent::currentAnimationFinished() const
{
	return false;
}
