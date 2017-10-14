#include "Animation2DComponent.h"
#include <algorithm>
#include <utility>

Ndk::ComponentIndex Animation2DComponent::componentIndex;

Animation2DComponent::Animation2DComponent(const Animation2D & animation)
	: m_animation(animation)
	, m_time(0)
	, m_paused(false)
{
}

const Animation2D & Animation2DComponent::getAnimation() const
{
	return m_animation;
}


Animation2D & Animation2DComponent::getAnimation()
{
	return m_animation;
}

const Frame & Animation2DComponent::updateAndGetFrame(float elapsedTime, bool * modified)
{
	bool changed = false;
	if(!m_paused)
		changed = addTime(elapsedTime);
	if (modified != nullptr)
		*modified = changed;
	return getCurrentFrame();
}

void Animation2DComponent::resetTime()
{
	setCurrentTime(0);
}

void Animation2DComponent::setCurrentTime(float time)
{
	m_time = time;
	updateCurrentFrame();
}

bool Animation2DComponent::addTime(float elapsedTime)
{
	m_time += elapsedTime;
	if (elapsedTime < m_currentFrame.time)
	{
		m_currentFrame.time -= elapsedTime;
		return false;
	}

	updateCurrentFrame();
	return true;
}

const Frame & Animation2DComponent::getCurrentFrame() const
{
	return m_currentFrame;
}

void Animation2DComponent::updateCurrentFrame()
{
	m_currentFrame = m_animation.getFrameAt(m_time);
}

void Animation2DComponent::pause()
{
	m_paused = true;
}

void Animation2DComponent::start()
{
	m_paused = false;
}

void Animation2DComponent::stop()
{
	m_paused = true;
	resetTime();
}

bool Animation2DComponent::isPaused() const
{
	return m_paused;
}

void Animation2DComponent::attach(Nz::SpriteRef sprite)
{
	if (std::find(m_sprites.begin(), m_sprites.end(), sprite) != m_sprites.end())
		return;
	m_sprites.push_back(sprite);
}

void Animation2DComponent::detach(Nz::SpriteRef sprite)
{
	m_sprites.erase(std::remove(m_sprites.begin(), m_sprites.end(), sprite), m_sprites.end());
}