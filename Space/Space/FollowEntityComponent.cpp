#include "FollowEntityComponent.h"

Ndk::ComponentIndex FollowEntityComponent::componentIndex;

FollowEntityComponent::FollowEntityComponent(Ndk::EntityHandle _entity, const Nz::Vector3f & _offset)
	: entity(_entity)
	, offset(_offset)
{

}
