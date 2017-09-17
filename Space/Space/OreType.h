#pragma once

#include <string>
#include <Nazara/Core/Error.hpp>

enum OreType
{
	OreType_Clay,
	OreType_Diamond,
	OreType_Emerald,
	OreType_Gold,
	OreType_Iron,
	OreType_Lapis,
	OreType_Netherrack,
	OreType_obsidian,
	OreType_Redstone,
	OreType_Yellow,

	OreType_Max = OreType_Yellow
};

std::string oreName(OreType type);
