
#include "OreType.h"

std::string oreName(OreType type)
{
	switch (type)
	{
	case OreType_Clay:
		return "clay";
	case OreType_Yellow:
		return "yellow";
	case OreType_Emerald:
		return "emerald";
	case OreType_Gold:
		return "gold";
	case OreType_Lapis:
		return "lapis";
	case OreType_obsidian:
		return "obsidian";
	case OreType_Iron:
		return "iron";
	case OreType_Redstone:
		return "redstone";
	case OreType_Diamond:
		return "diamond";
	case OreType_Netherrack:
		return "netherrack";
	default:
		NazaraAssert(false, "No texture defined for this type.");
		break;
	}

	return "";
}