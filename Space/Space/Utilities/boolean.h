#ifndef BOOLEAN_H
#define BOOLEAN_H

#include <Nazara/Math/Vector2.hpp>
#include <vector>

enum class BooleanType
{
    SUBSTRACT,
    ADD,
    INTERSECTION,
};

using Shape = std::vector<Nz::Vector2f>;

Shape createCircularShape(float radius, unsigned int sides);

std::vector<Shape> boolean(const Shape & a, const Shape & b, const Nz::Vector2f & offset, BooleanType type);

Nz::Vector2f centerShape(const Shape & s);
bool isClockWise(const Shape & s);
Shape switchPointsOrder(const Shape & s);
void switchPointsOrder(Shape & s);
bool isCrossed(const Shape & s);

Nz::Vector2f nearestPointOn(const Nz::Vector2f & pos, const Shape & s);

#endif // BOOLEAN_H
