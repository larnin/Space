#ifndef VECT2EXT_H
#define VECT2EXT_H

#include <Nazara/Math/Vector2.hpp>

float angle(const Nz::Vector2f & vect);
float angle(const Nz::Vector2f &vect1, const Nz::Vector2f & vect2); //between 0 and pi
float signedAngle(const Nz::Vector2f &vect1, const Nz::Vector2f & vect2);
float norm(const Nz::Vector2f & vect);
float normSqr(const Nz::Vector2f & vect);
Nz::Vector2f normalize(const Nz::Vector2f & vect);

Nz::Vector2f toVect(float norm, float angle);
Nz::Vector2f rotate(const Nz::Vector2f & vect, float rotateAngle);

float dot(const Nz::Vector2f & a, const Nz::Vector2f & b);

Nz::Vector2f ortho(const Nz::Vector2f & v);

bool isLeft(const Nz::Vector2f & p1, const Nz::Vector2f & p2, const Nz::Vector2f & pos);
bool isRight(const Nz::Vector2f & p1, const Nz::Vector2f & p2, const Nz::Vector2f & pos);

bool interactSergment(const Nz::Vector2f & a, const Nz::Vector2f & b, const Nz::Vector2f & i, const Nz::Vector2f & j, Nz::Vector2f * out = nullptr);

#endif // VECT2CONVERT_H
