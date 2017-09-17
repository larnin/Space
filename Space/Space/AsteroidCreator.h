#pragma once

#include "OreType.h"
#include <Nazara/Renderer/Texture.hpp>
#include <NDK/Entity.hpp>
#include <NDK/World.hpp>
#include <map>

struct AsteroidParameters
{
	unsigned int seed = 0;
	unsigned int subdivisions = 3;
	Nz::Vector3f sphereScale = Nz::Vector3f(1, 1, 1);
	float period = 2;
	float amplitude = 1.0f;
	float amplitudeMultiplier = 0.5f;
	float amplitudeExp = 1.0f;
	int steps = 3;
	float scale = 1.0f;
	float scaleMultiplier = 2.0f;
	float damageResistance = 1.0f;
	OreType oreType;
};

class AsteroidCreator
{
public:
	AsteroidCreator(Ndk::World & world);
	~AsteroidCreator() = default;

	Ndk::EntityHandle create(const AsteroidParameters & params, const Nz::Vector3f & pos);

private:
	void loadTextures();

	Nz::TextureRef OreTypeToStoneTexture(OreType type);
	Nz::TextureRef OreTypeToBrokenStoneTexture(OreType type);
	Nz::TextureRef OreTypeToOreTexture(OreType type);

	enum TextureIndex
	{
		TextureIndex_Stone,
		TextureIndex_Lightstone,
		TextureIndex_Sandstone,
		TextureIndex_Bedrock,
		TextureIndex_Iron2,
		TextureIndex_Clay,
		TextureIndex_Diamond,
		TextureIndex_Emerald,
		TextureIndex_Gold,
		TextureIndex_Iron,
		TextureIndex_Lapis,
		TextureIndex_Netherrack,
		TextureIndex_Obsidian,
		TextureIndex_Redstone,
		TextureIndex_Yellow
	};

	std::map<int, Nz::TextureRef> m_textures;
	Ndk::World& m_world;
};

