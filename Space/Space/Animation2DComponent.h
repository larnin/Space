#pragma once

#include "Animation2D.h"
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

	const Frame & updateAndGetFrame(float elapsedTime, bool * modified = nullptr);
	void resetTime();
	void setCurrentTime(float time);
	bool addTime(float elapsedTime);
	const Frame & getCurrentFrame() const;
	void pause();
	void start();
	void stop();
	bool isPaused() const;
	void attach(Nz::SpriteRef sprite);
	void detach(Nz::SpriteRef sprite);

	static Ndk::ComponentIndex componentIndex;

private:
	void updateCurrentFrame();

	Animation2D m_animation;
	float m_time;
	Frame m_currentFrame;
	bool m_paused;
	std::vector<Nz::SpriteRef> m_sprites;
};

