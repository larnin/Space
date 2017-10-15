#pragma once

#include "Animation2D/Animation2D.h"
#include <Nazara/Graphics/Sprite.hpp>
#include <NDK/Component.hpp>
#include <vector>

class Animation2DComponent : public Ndk::Component<Animation2DComponent>
{
	friend class Animation2DSystem;

public:
	Animation2DComponent(const Animation2D & animation);
	~Animation2DComponent() = default;

	const Animation2D & getAnimation() const;
	Animation2D & getAnimation();
	void setAnimation(const Animation2D & animation, bool resetTime = true);

	void resetTime();
	void setCurrentTime(float time);
	//return true if the frame have changed from the last addTime
	bool update(float elapsedTime);
	void setAnimationSpeed(float value);
	float getAnimationSpeed() const;
	const Frame & getCurrentFrame() const;
	void pause();
	void start();
	void stop();
	void restart();
	bool isPaused() const;
	void attach(Nz::SpriteRef sprite);
	void detach(Nz::SpriteRef sprite);
	void detachAll();

	static void setFrame(Nz::SpriteRef & sprite, const Frame & f);

	static Ndk::ComponentIndex componentIndex;

private:
	Animation2D m_animation;
	float m_time;
	float m_currentFrameTime;
	float m_speed;
	size_t m_currentFrameIndex;
	bool m_paused;
	bool m_changedState;
	std::vector<Nz::SpriteRef> m_sprites;
};

