#include "Components/Animation2DComponent.h"
#include <algorithm>
#include <utility>

Ndk::ComponentIndex Animation2DComponent::componentIndex;

Animation2DComponent::Animation2DComponent(const Animation2D & animation)
	: m_animation(animation)
	, m_time(0)
	, m_currentFrameTime(0)
	, m_speed(1)
	, m_currentFrameIndex(0)
	, m_paused(false)
	, m_changedState(true)
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

void Animation2DComponent::setAnimation(const Animation2D & animation, bool resetTime)
{
	m_changedState = true;
	m_animation = animation;
	if (resetTime)
	{
		m_time = 0;
		m_currentFrameIndex = 0;
		m_currentFrameTime = 0;
	}
	else
		setCurrentTime(m_time);
}

void Animation2DComponent::resetTime()
{
	setCurrentTime(0);
}

void Animation2DComponent::setCurrentTime(float time)
{
	m_time = m_animation.toNormalizedTime(time);
	m_changedState = true;

	float current = 0;
	for (size_t i(0); i < m_animation.frameCount(); i++)
	{
		const auto & f = m_animation[i];
		current += f.time;
		if (current > m_time)
		{
			m_currentFrameTime = current - m_time;
			m_currentFrameIndex = i;
			return;
		}
	}
	m_currentFrameTime = 0;
	m_currentFrameIndex = m_animation.frameCount() - 1;
}
#include <iostream>
bool Animation2DComponent::update(float elapsedTime)
{
	elapsedTime *= m_speed;

	bool oldChangedState = m_changedState;
	m_changedState = false;

	if (m_paused)
		return oldChangedState;

	m_time += elapsedTime;
	m_currentFrameTime += elapsedTime;

	while(m_currentFrameTime < 0)
	{
		if (m_animation.isSingleShoot() && m_currentFrameIndex == 0)
			return oldChangedState;

		m_currentFrameIndex = m_currentFrameIndex == 0 ? m_animation.frameCount() - 1 : m_currentFrameIndex - 1;
		m_currentFrameTime += m_animation[m_currentFrameIndex].time;
		return true;
	}
	while(m_currentFrameTime > m_animation[m_currentFrameIndex].time)
	{
		if(m_animation.isSingleShoot() && m_currentFrameIndex == m_animation.frameCount() - 1)
			return oldChangedState;

		m_currentFrameTime -= m_animation[m_currentFrameIndex].time;
		m_currentFrameIndex = m_currentFrameIndex == m_animation.frameCount() - 1 ? 0 : m_currentFrameIndex + 1;
		return true;
	}
	return oldChangedState;
}

void Animation2DComponent::setAnimationSpeed(float value)
{
	m_speed = value;
}

float Animation2DComponent::getAnimationSpeed() const
{
	return m_speed;
}

const Frame & Animation2DComponent::getCurrentFrame() const
{
	return m_animation[m_currentFrameIndex];
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

void Animation2DComponent::restart()
{
	m_paused = false;
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
	setFrame(sprite, m_animation[m_currentFrameIndex]);
}

void Animation2DComponent::detach(Nz::SpriteRef sprite)
{
	m_sprites.erase(std::remove(m_sprites.begin(), m_sprites.end(), sprite), m_sprites.end());
}

void Animation2DComponent::detachAll()
{
	m_sprites.clear();
}

void Animation2DComponent::setFrame(Nz::SpriteRef & sprite, const Frame & f)
{
	sprite->SetOrigin(Nz::Vector3f(f.offset, 0));

	const auto & material = sprite->GetMaterial();
	NazaraAssert(material->HasDiffuseMap(), "Sprite material has no diffuse map");

	const auto & diffuseMap = material->GetDiffuseMap();

	float invWidth = 1.f / diffuseMap->GetWidth();
	float invHeight = 1.f / diffuseMap->GetHeight();

	Nz::Rectf rect(invWidth * f.texRect.x, invHeight * f.texRect.y, invWidth * f.texRect.width, invHeight * f.texRect.height);
	if (f.xFliped)
	{
		rect.x += rect.width;
		rect.width *= -1;
	}
	if (f.yFliped)
	{
		rect.y += rect.height;
		rect.height *= -1;
	}

	sprite->SetTextureCoords(rect);
	sprite->SetSize(f.texRect.width, f.texRect.height);
}