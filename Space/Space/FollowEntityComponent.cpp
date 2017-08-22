#include "FollowEntityComponent.h"

Ndk::ComponentIndex FollowEntityComponent::componentIndex;

FollowEntityComponent::FollowEntityComponent(Ndk::EntityHandle _entity)
	: entity(_entity)
{

}
