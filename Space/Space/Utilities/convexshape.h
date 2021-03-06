#ifndef CONVEXSHAPE_H
#define CONVEXSHAPE_H

#include <Nazara/Math/Vector2.hpp>
#include <vector>

using Shape = std::vector<Nz::Vector2f>;

bool isConvexe(const Shape & s);
std::vector<Shape> makeConvexe(const Shape & s);
float calculateSurface(const Shape & s);
bool isInShape(const Shape & s, const Nz::Vector2f & pos);

#endif // CONVEXSHAPE_H
