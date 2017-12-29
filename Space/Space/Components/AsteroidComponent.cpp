#include "AsteroidComponent.h"
#include "Utilities/convexshape.h"
#include "Utilities/boolean.h"
#include <NDK/Components/GraphicsComponent.hpp>
#include <Nazara/Physics2D/Collider2D.hpp>

Ndk::ComponentIndex AsteroidComponent::componentIndex;

AsteroidComponent::AsteroidComponent(const AsteroidBaseInfosRef & baseInfos)
	: m_baseInfos(baseInfos)
	, m_shape(baseInfos->shape)
	, m_image(Nz::Image::New())
	, m_sprite(Nz::Sprite::New(Nz::Material::New("Translucent2D")))
	, m_physicComponent(nullptr)
	, m_collisionComponent(nullptr)
{
	auto & sampler = m_sprite->GetMaterial()->GetDiffuseSampler();
	sampler.SetFilterMode(Nz::SamplerFilter_Nearest);

	initializeObject();
	updateObject();
}

void AsteroidComponent::damage(const Nz::Vector2f & pos, float radius)
{

}

void AsteroidComponent::initializeObject()
{
	auto entity = GetEntity();
	auto & graphic = entity->AddComponent<Ndk::GraphicsComponent>();
	graphic.Attach(m_sprite);
	m_physicComponent = &entity->AddComponent<Ndk::PhysicsComponent2D>();
	m_collisionComponent = &entity->AddComponent<Ndk::CollisionComponent2D>();
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
	createRender(*m_image, *m_baseInfos->asteroid, *m_baseInfos->explosion, *m_baseInfos->border, m_baseInfos->data, m_explosions);
	m_sprite->GetMaterial()->GetDiffuseMap()->Download(m_image);
}
