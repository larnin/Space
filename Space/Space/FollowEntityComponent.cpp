#include "FollowEntityComponent.h"

Ndk::ComponentIndex FollowEntityComponent::componentIndex;

FollowEntityComponent::FollowEntityComponent(Ndk::EntityHandle entity, const Nz::Vector3f & offset)
	: m_entity(entity)
	, m_offset(offset)
{

}
