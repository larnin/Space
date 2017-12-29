#pragma once

#include "Utilities/AsteroidBaseInfos.h"
#include "Utilities/AsteroidRender.h"
#include <NDK/Component.hpp>
#include <NDK/Components/CollisionComponent2D.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Graphics/Sprite.hpp>

using Shape = std::vector<Nz::Vector2f>;

class AsteroidComponent : public Ndk::Component<AsteroidComponent>
{
public:
	AsteroidComponent(const AsteroidBaseInfosRef & baseInfos);

	virtual ~AsteroidComponent() = default;

	void damage(const Nz::Vector2f & pos, float radius);

	static Ndk::ComponentIndex componentIndex;

private:
	void initializeObject();
	void updateObject();

	AsteroidBaseInfosRef m_baseInfos;
	Shape m_shape;
	std::vector<Shape> m_convexeShapes;
	std::vector<AsteroidExplosion> m_explosions;
	Nz::ImageRef m_image;
	Nz::SpriteRef m_sprite;
	Ndk::CollisionComponent2D* m_collisionComponent;
	Ndk::PhysicsComponent2D* m_physicComponent;
};

