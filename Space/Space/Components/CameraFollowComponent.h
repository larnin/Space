#pragma once
#include <NDK/Component.hpp>
#include <NDK/Entity.hpp>
#include "CameraFollowControlerComponent.h"

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
	Ndk::EntityHandle m_target;
};

