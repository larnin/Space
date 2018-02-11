#include "AsteroidComponent.h"
#include "Utilities/convexshape.h"
#include "Utilities/boolean.h"
#include "Utilities/vect2ext.h"
#include <NDK/Components/GraphicsComponent.hpp>
#include <Nazara/Physics2D/Collider2D.hpp>

#include <iostream>

Ndk::ComponentIndex AsteroidComponent::componentIndex;

const float minimumViableSurface = 100.0f;

AsteroidComponent::AsteroidComponent(const AsteroidBaseInfosRef & baseInfos)
	: m_baseInfos(baseInfos)
	, m_shape(baseInfos->shape)
	, m_image(Nz::Image::New())
	, m_sprite(Nz::Sprite::New(Nz::Material::New("Translucent2D")))
	, m_physicComponent(nullptr)
	, m_collisionComponent(nullptr)
	, m_nodeComponent(nullptr)
{
	auto & sampler = m_sprite->GetMaterial()->GetDiffuseSampler();
	sampler.SetFilterMode(Nz::SamplerFilter_Nearest);
	auto texture = Nz::Texture::New(*(m_baseInfos->asteroid));
	m_sprite->SetTexture(texture);
	m_sprite->SetSize(texture->GetWidth(), texture->GetHeight());
}

void AsteroidComponent::damage(Nz::Vector2f pos, float radius, float force)
{
	pos -= m_physicComponent->GetPosition();
	pos = rotate(pos, -m_physicComponent->GetRotation());

	auto contactPos = nearestPointOn(pos, m_shape);

	auto shapes = boolean(m_shape, createCircularShape(radius, std::sqrt(radius + 4)), pos, BooleanType::SUBSTRACT);
	m_explosions.emplace_back(radius, Nz::Vector2i(pos.x, pos.y));
	if (shapes.empty())
	{
		destroy();
		return;
	}
	else if (shapes.size() > 1)
	{
		split(shapes, contactPos, normalize(contactPos - pos) * force);
		return;
	}

	m_shape = shapes[0];
	updateObject();
	m_physicComponent->AddImpulse(normalize(contactPos - pos) * force, contactPos, Nz::CoordSys::CoordSys_Local);
}

Ndk::EntityHandle AsteroidComponent::create(Ndk::World & w, const AsteroidBaseInfosRef & baseInfos, const Nz::Vector2f & pos, float rot, const Nz::Vector2f & velocity, float angularVelocity)
{
	auto e = w.CreateEntity();
	auto & asteroid = e->AddComponent<AsteroidComponent>(baseInfos);
	asteroid.initializeObject(pos, rot, velocity, angularVelocity);
	return e;
}

void AsteroidComponent::initializeObject(const Nz::Vector2f & pos, float rot, const Nz::Vector2f & velocity, float angularVelocity)
{
	auto entity = GetEntity();
	auto & graphic = entity->AddComponent<Ndk::GraphicsComponent>();
	graphic.Attach(m_sprite);
	m_nodeComponent = &entity->AddComponent<Ndk::NodeComponent>();
	m_collisionComponent = &entity->AddComponent<Ndk::CollisionComponent2D>();
	m_physicComponent = &entity->AddComponent<Ndk::PhysicsComponent2D>();
	m_physicComponent->SetPosition(pos);
	m_physicComponent->SetRotation(rot);
	m_physicComponent->SetVelocity(velocity);
	m_physicComponent->SetAngularVelocity(angularVelocity); 
	
	updateObject();
}

void AsteroidComponent::updateObject()
{
	m_convexeShapes = makeConvexe(m_shape);

	std::vector<Nz::Collider2DRef> colliders;
	for (const auto & shape : m_convexeShapes)
		colliders.push_back(Nz::ConvexCollider2D::New(shape.data(), shape.size()));
	auto collider = Nz::CompoundCollider2D::New(colliders);
	m_collisionComponent->SetGeom(collider);
	m_physicComponent->SetMassCenter(centerShape(m_shape));
	float mass = 0;
	for (const auto & s : m_convexeShapes)
		mass += calculateSurface(s);
	if (mass < minimumViableSurface)
		destroy();
	m_physicComponent->SetMass(mass);
	createRender(*m_image, *m_baseInfos->asteroid, *m_baseInfos->explosion, *m_baseInfos->border, m_baseInfos->data, m_explosions);
	m_sprite->GetMaterial()->GetDiffuseMap()->Update(*m_image);
}

void AsteroidComponent::split(const std::vector<Shape>& shapes, const Nz::Vector2f & explosionPos, const Nz::Vector2f & impulsion)
{
	assert(shapes.size() > 1 && "It must have at least 2 shapes in the vector");

	auto baseInfo = m_baseInfos;
	m_baseInfos = AsteroidBaseInfos::New(*baseInfo);//clone
	m_baseInfos->asteroid = Nz::Image::New();
	removePixelsOutOfShape(*(m_baseInfos->asteroid), *(baseInfo->asteroid), shapes[0]);

	for (unsigned int i(1); i < shapes.size(); i++)
	{
		auto bi = AsteroidBaseInfos::New(*baseInfo);
		bi->asteroid = Nz::Image::New();
		removePixelsOutOfShape(*(bi->asteroid), *(baseInfo->asteroid), shapes[i]);
		auto e = create(*GetEntity()->GetWorld(), bi, m_physicComponent->GetPosition(), m_physicComponent->GetRotation(), m_physicComponent->GetVelocity(), m_physicComponent->GetAngularVelocity());
		auto & phys = e->GetComponent<Ndk::PhysicsComponent2D>();
		phys.AddImpulse(impulsion, explosionPos, Nz::CoordSys::CoordSys_Local);
	}

	updateObject();
	m_physicComponent->AddImpulse(impulsion, explosionPos, Nz::CoordSys::CoordSys_Local);
}

void AsteroidComponent::destroy()
{
	GetEntity()->Kill();
}
