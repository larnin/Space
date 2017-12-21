#include "asteroidrender.h"
#include "vect2ext.h"
#include <Nazara/Noise/Perlin.hpp>
#include <cassert>

#include <iostream>
#include <Nazara/Core/Clock.hpp>

void createHole(Nz::Image & image, const Nz::Vector2i & localPos, float size, const Nz::Image & border)
{
	assert(image.GetFormat() == Nz::PixelFormatType::PixelFormatType_RGBA8 && "The image must have RGBA8 pixel format type");
	assert(border.GetFormat() == Nz::PixelFormatType::PixelFormatType_RGBA8 && "The border image must have RGBA8 pixel format type");

	Nz::Clock c;

	static Nz::Perlin perlin;
	const float perlinMultiplier = 0.2f;
	const float perlinScale = 0.1f;

	float borderSize = float(border.GetSize().y) / border.GetSize().x * 2 * M_PI * size;
	float borderMultiplier = float(border.GetSize().y) / borderSize;
	float angleMultiplier = float(border.GetSize().x) / 2 / M_PI;
	float sizeSqr = size * size;
	float fullSizeSqr = (size + borderSize)*(size + borderSize);

	for(int i(- size - borderSize) ; i < size + borderSize ; i++)
		for (int j(-size - borderSize); j < size + borderSize; j++)
		{
			Nz::Vector2i pos(localPos + Nz::Vector2i(i, j));
			if (pos.x < 0 || pos.x >= image.GetSize().x || pos.y < 0 || pos.y >= image.GetSize().y)
				continue;

			float dist = ((i*i) + (j*j)) * (1 + perlin.Get(pos.x, pos.y, perlinScale) * perlinMultiplier);
			if (dist >= fullSizeSqr)
				continue;
			if (dist < sizeSqr)
				image.GetPixels(pos.x, pos.y)[3] = 0; //set alpha to 0
			else
			{
				float d = (std::sqrt(dist) - size) * borderMultiplier;
				float a = (angle(Nz::Vector2f(i, j)) + M_PI) * angleMultiplier;
				if (a >= border.GetSize().x)
					a = 0;
				auto fromColor = image.GetPixels(pos.x, pos.y);
				auto multiplierColor = border.GetConstPixels(a, d);
				for (unsigned int k(0); k < 4; k++)
					fromColor[k] = static_cast<Nz::UInt8>((static_cast<unsigned int>(fromColor[k]) * static_cast<unsigned int>(multiplierColor[k])) / 255U);
			}
		}

	std::cout << c.GetSeconds() << std::endl;
}
