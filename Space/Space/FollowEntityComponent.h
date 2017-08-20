#pragma once

#include <NDK/Component.hpp>
#include <NDK/Entity.hpp>
#include <Nazara/Math/Vector3.hpp>

class FollowEntityComponent : public Ndk::Component<FollowEntityComponent>
{
public:
	FollowEntityComponent(Ndk::EntityHandle _entity, const Nz::Vector3f & _offset);
	~FollowEntityComponent() = default;

	static Ndk::ComponentIndex componentIndex;

	Ndk::EntityHandle entity;
	Nz::Vector3f offset;
	float velocityOffsetMultiplier;

private:
};

