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

std::vector<Shape> boolean(const Shape & a, const Shape & b, const Nz::Vector2f & offset, BooleanType type);

bool isClockWise(const Shape & s);
Shape switchPointsOrder(const Shape & s);
void switchPointsOrder(Shape &s);
bool isCrossed(const Shape & s);

#endif // BOOLEAN_H
