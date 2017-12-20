#include "asteroidrender.h"
#include "vect2ext.h"

#include <iostream>

void createHole(Nz::Image & image, const Nz::Vector2i & localPos, float size, const Nz::Image & border)
{
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

			float dist = (i*i) + (j*j);
			if (dist >= fullSizeSqr)
				continue;
			if (dist < sizeSqr)
				image.SetPixelColor(Nz::Color(0, 0, 0, 0), pos.x, pos.y);
			else
			{
				float d = (std::sqrt(dist) - size) * borderMultiplier;
				float a = (angle(Nz::Vector2f(i, j)) + M_PI) * angleMultiplier;
				if (a >= border.GetSize().x)//on the borned value
					a = 0;
				image.SetPixelColor(image.GetPixelColor(pos.x, pos.y) * border.GetPixelColor(a, d), pos.x, pos.y);
			}
		}
}
