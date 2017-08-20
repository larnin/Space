#pragma once

#include <NDK/Component.hpp>
#include <NDK/Entity.hpp>
#include <Nazara/Math/Vector3.hpp>

class FollowEntityComponent : public Ndk::Component<FollowEntityComponent>
{
public:
	FollowEntityComponent(Ndk::EntityHandle entity, const Nz::Vector3f & offset);
	~FollowEntityComponent() = default;

	static Ndk::ComponentIndex componentIndex;

private:
	Ndk::EntityHandle m_entity;
	Nz::Vector3f m_offset;
};

