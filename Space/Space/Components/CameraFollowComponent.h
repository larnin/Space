#pragma once
#include "CameraFollowControlerComponent.h"
#include <NDK/Component.hpp>
#include <NDK/Entity.hpp>
#include <vector>

class CameraFollowComponent : public Ndk::Component<CameraFollowComponent>
{
public:
	CameraFollowComponent(Ndk::EntityHandle target);
	virtual ~CameraFollowComponent() = default;

	void setTarget(Ndk::EntityHandle target);
	Ndk::EntityHandle getTarget() const;

	void update(const CommandsInfos & commands, float elapsedTime);

	static Ndk::ComponentIndex componentIndex;

private:
	static std::vector<float> m_zoomLevels;

	Ndk::EntityHandle m_target;
	unsigned int m_currentZoom;
};

