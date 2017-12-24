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

AsteroidData createAsteroidData(const Nz::Image & asteroid)
{
	assert(asteroid.GetFormat() == Nz::PixelFormatType::PixelFormatType_RGBA8 && "The image must have RGBA8 pixel format type");

	std::vector<Nz::Vector2ui> borders;
	for(unsigned int i(0) ; i < asteroid.GetWidth() ; i++)
		for (unsigned int j(0); j < asteroid.GetHeight(); j++)
		{
			if (asteroid.GetConstPixels(i, j)[3] < 128) //alpha
				continue;

			std::vector<Nz::Vector2i> dirs{ {-1, 0},{1, 0},{0, -1},{0, 1} };
			for (const auto & d : dirs)
			{
				Nz::Vector2i p(i + d.x, j + d.y);
				if (p.x < 0 || p.y < 0 || p.x >= asteroid.GetWidth() || p.y >= asteroid.GetHeight() || asteroid.GetConstPixels(p.x, p.y)[3] < 128)
				{
					borders.emplace_back(i, j);
						break;
				}
			}
		}

	AsteroidData data(asteroid.GetWidth(), asteroid.GetHeight(), 0);
	auto f = [](float x, float a) {return -a / (x + a) + 1; };

	for (unsigned int i(0); i < asteroid.GetWidth(); i++)
		for (unsigned int j(0); j < asteroid.GetHeight(); j++)
		{
			if (asteroid.GetConstPixels(i, j)[3] < 128) //alpha
				continue;
			float bestDist = std::numeric_limits<float>::max();
			for (const auto & b : borders)
			{
				float d = (b - Nz::Vector2ui(i, j)).GetSquaredLength();
				if (d < bestDist)
					bestDist = d;
			}
			data(i, j) = f(std::sqrt(bestDist), 20);
		}
	return data;
}

void createRender(Nz::Image & target, const Nz::Image & asteroid, const Nz::Image & explosion, const Nz::Image & border, const AsteroidData & data, const std::vector<AsteroidExplosion>& explosions)
{
	assert(asteroid.GetFormat() == Nz::PixelFormatType::PixelFormatType_RGBA8 && "The image asteroid must have RGBA8 pixel format type");
	assert(explosion.GetFormat() == Nz::PixelFormatType::PixelFormatType_RGBA8 && "The image explosion must have RGBA8 pixel format type");
	assert(border.GetFormat() == Nz::PixelFormatType::PixelFormatType_RGBA8 && "The image border must have RGBA8 pixel format type");
	assert(data.width() == asteroid.GetWidth() && data.height() == asteroid.GetHeight() && "The AsteroidData size is not the same as the asteroid image");

	if (!target.IsValid() || target.GetSize() != asteroid.GetSize() || target.GetFormat() != Nz::PixelFormatType::PixelFormatType_RGBA8)
		target.Create(Nz::ImageType::ImageType_2D, Nz::PixelFormatType::PixelFormatType_RGBA8, asteroid.GetWidth(), asteroid.GetHeight());

	const float borderAngleMultiplier = float(border.GetWidth()) / 2 / M_PI;
	const float explosionAngleMultiplier = float(explosion.GetWidth()) / 2 / M_PI;

	static Nz::Perlin perlin;
	const float perlinMultiplier = 0.12f;
	const float perlinScale = 0.1f;

	unsigned int explosedCount = 0;
	unsigned int onExplosionCount = 0;
	unsigned int onBorderCount = 0;
	unsigned int unchangedCount = 0;

	for (unsigned int i(0); i < target.GetWidth(); i++)
		for (unsigned int j(0); j < asteroid.GetHeight(); j++)
		{
			bool explosed = false;
			bool onExplosion = false;
			bool onBorder = false;
			float normalizedDist = 1;
			float angleDir = 0;
			auto pixel = asteroid.GetConstPixels(i, j);
			if (pixel[3] < 128)
			{
				target.GetPixels(i, j)[3] = 0;
				continue;
			}

			for (const auto & e : explosions)
			{
				auto dir = e.localPos - Nz::Vector2i(i, j);
				auto d = dir.GetLength() * (1 + perlin.Get(i, j, perlinScale) * perlinMultiplier);
				if (d < e.radius)
				{
					explosed = true;
					break;
				}

				d -= e.radius;

				float explosionSize = float(explosion.GetHeight()) / explosion.GetWidth() * 2 * M_PI * e.radius;
				auto p = e.localPos - Nz::Vector2i(Nz::Vector2f(dir * e.radius) / dir.GetLengthf());
				if (p.x < 0 || p.x >= data.width() || p.y < 0 || p.y >= data.height())
					explosionSize *= 0;
				else explosionSize *= data(p.x, p.y);
				if (d < explosionSize)
				{
					if (!onExplosion)
						normalizedDist = 1;
					onExplosion = true;
					auto nD = d / explosionSize;
					if (nD < normalizedDist)
					{
						normalizedDist = nD;
						angleDir = angle(Nz::Vector2f(dir.x, dir.y));
					}
					continue;
				}
				if (onExplosion)
					continue;
				d -= explosionSize;

				float borderSize = float(border.GetHeight()) / border.GetWidth() * 2 * M_PI * e.radius;
				if (d < borderSize)
				{
					onBorder = true;
					auto nD = d / borderSize;
					if (nD < normalizedDist)
					{
						normalizedDist = nD;
						angleDir = angle(Nz::Vector2f(dir.x, dir.y));
					}
				}
			}

			if (explosed)
			{
				target.GetPixels(i, j)[3] = 0;
				explosedCount++;
			}
			else if (onExplosion)
			{
				float d = explosion.GetHeight() * normalizedDist;
				float a = (angle(Nz::Vector2f(i, j)) + M_PI) * explosionAngleMultiplier;
				if (a >= explosion.GetSize().x)
					a = 0;
				if (d >= explosion.GetHeight())
					d = explosion.GetHeight() - 1;
				auto destColor = target.GetPixels(i, j);
				auto color = explosion.GetConstPixels(a, d);
				for (unsigned int k(0); k < 4; k++)
					destColor[k] = color[k];
				onExplosionCount++;
			}
			else if (onBorder)
			{
				float d = border.GetHeight() * normalizedDist;
				float a = (angle(Nz::Vector2f(i, j)) + M_PI) * borderAngleMultiplier;
				if (a >= border.GetSize().x)
					a = 0;
				auto destColor = target.GetPixels(i, j);
				auto borderColor = border.GetConstPixels(a, d);
				for (unsigned int k(0); k < 4; k++)
					destColor[k] = static_cast<Nz::UInt8>((static_cast<unsigned int>(pixel[k]) * static_cast<unsigned int>(borderColor[k])) / 255U);
				onBorderCount++;
			}
			else
			{
				auto destColor = target.GetPixels(i, j);
				for (unsigned int k(0); k < 4; k++)
					destColor[k] = pixel[k];
				unchangedCount++;
			}
		}
}
