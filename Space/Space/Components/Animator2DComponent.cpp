#include "Animator2DComponent.h"

Ndk::ComponentIndex Animator2DComponent::componentIndex;

Animator2DComponent::Animator2DComponent(Animator2DRef animator, float animationSpeed)
	: m_animator(animator)
	, m_currentStateTime(0)
	, m_animationSpeed(animationSpeed)
	, m_paused(false)
	, m_needToUpdate(true)
{
	resetAnimator();
}

void Animator2DComponent::resetAnimator()
{
	if (m_animator->stateExist(m_animator->getDefaultStateName()))
		m_currentState = &m_animator->getState(m_animator->getDefaultStateName());
	m_properties.reset();
	updateCurrentFrame();
	m_needToUpdate = true;
}

bool Animator2DComponent::update(float elapsedTime)
{
	bool oldNeedToUpdate = m_needToUpdate;
	m_needToUpdate = false;

	if (m_currentState == nullptr)
		return m_needToUpdate;

	if (checkTransitions())
		return true;

	if (m_paused)
		return m_needToUpdate;
	elapsedTime *= std::abs(m_animationSpeed * m_currentState->getSpeed());

	m_currentStateTime += elapsedTime;
	m_currentFrame.time -= elapsedTime;
	if (m_currentFrame.time < 0)
	{
		updateCurrentFrame();
		return true;
	}
	return m_needToUpdate;
}

void Animator2DComponent::setAnimationSpeed(float value)
{
	m_animationSpeed = value;
}

float Animator2DComponent::getAnimationSpeed() const
{
	return m_animationSpeed;
}

const Frame & Animator2DComponent::getCurrentFrame() const
{
	return m_currentFrame;
}

void Animator2DComponent::pause()
{
	m_paused = true;
}

void Animator2DComponent::start()
{
	m_paused = false;
}

void Animator2DComponent::stop()
{
	m_paused = true;
	resetAnimator();
}

bool Animator2DComponent::isPaused() const
{
	return m_paused;
}

void Animator2DComponent::attach(Nz::SpriteRef sprite)
{
	if (std::find(m_sprites.begin(), m_sprites.end(), sprite) != m_sprites.end())
		return;
	m_sprites.push_back(sprite);
}

void Animator2DComponent::detach(Nz::SpriteRef sprite)
{
	m_sprites.erase(std::remove(m_sprites.begin(), m_sprites.end(), sprite), m_sprites.end());
}

void Animator2DComponent::detachAll()
{
	m_sprites.clear();
}

void Animator2DComponent::setProperty(const std::string & key, int value)
{
	m_properties.set(key, value);
}

int Animator2DComponent::getProperty(const std::string & key) const
{
	return m_properties.get(key);
}

bool Animator2DComponent::currentAnimationFinished() const
{
	if (m_currentState == nullptr)
		return false;

	return m_currentStateTime >= m_currentState->getAnimation()->getTotalAnimationTime();
}

void Animator2DComponent::updateCurrentFrame()
{
	if (m_currentState != nullptr)
	{
		float speed = m_currentState->getSpeed() * m_animationSpeed;
		m_currentFrame = m_currentState->getAnimation()->getFrameAt(speed > 0 ? m_currentStateTime : m_currentState->getAnimation()->getTotalAnimationTime() - m_currentStateTime);
		m_currentFrame.xFliped = m_currentFrame.xFliped != m_currentState->getXfliped();
		m_currentFrame.yFliped = m_currentFrame.yFliped != m_currentState->getYFliped();
	}
}

bool Animator2DComponent::checkTransitions()
{
	Animation2DEnv env(m_currentStateTime, currentAnimationFinished(), m_properties);

	for (int i = 0; i < m_currentState->transitionCount(); i++)
	{
		if (m_currentState->transition(i).check(env))
		{
			m_currentState = &m_currentState->transition(i).getDest();
			m_currentStateTime = 0;
			updateCurrentFrame();
			return true;
		}
	}

	return false;
}

void Animator2DComponent::setFrame(Nz::SpriteRef & sprite, const Frame & f)
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
