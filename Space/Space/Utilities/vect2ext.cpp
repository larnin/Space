#include "vect2ext.h"
#include <cmath>

float angle(const Nz::Vector2f & vect)
{
    return atan2(vect.y,vect.x);
}

float angle(const Nz::Vector2f &vect1, const Nz::Vector2f & vect2)
{
    return std::acos(dot(normalize(vect1), normalize(vect2)));
}

float signedAngle(const Nz::Vector2f &vect1, const Nz::Vector2f & vect2)
{
    return angle(vect2) - angle(vect1);
}

float norm(const Nz::Vector2f & vect)
{
    return sqrt(vect.x*vect.x +vect.y*vect.y);
}

Nz::Vector2f normalize(const Nz::Vector2f & vect)
{
    return vect/norm(vect);
}

Nz::Vector2f toVect(float norm, float angle)
{
    return Nz::Vector2f(cos(angle)*norm, sin(angle)*norm);
}

Nz::Vector2f rotate(const Nz::Vector2f & vect, float rotateAngle)
{
    return toVect(norm(vect), angle(vect) + rotateAngle);
}

float dot(const Nz::Vector2f & a, const Nz::Vector2f & b)
{
    return a.x*b.x + a.y*b.y;
}

Nz::Vector2f ortho(const Nz::Vector2f & v)
{
    return Nz::Vector2f(v.y, -v.x);
}

bool isLeft(const Nz::Vector2f & p1, const Nz::Vector2f & p2, const Nz::Vector2f & pos)
{
    return dot(p2 - p1, ortho(pos - p2)) > 0;
}

bool isRight(const Nz::Vector2f &p1, const Nz::Vector2f &p2, const Nz::Vector2f & pos)
{
    return dot(p2 - p1, ortho(pos - p2)) < 0;
}

bool interactSergment(const Nz::Vector2f & a, const Nz::Vector2f & b, const Nz::Vector2f & i, const Nz::Vector2f & j, Nz::Vector2f *out)
{
	Nz::Vector2f sPos(b-a), sSeg(j-i);
    float denom(sPos.x*sSeg.y - sPos.y*sSeg.x);
    if(denom == 0)
        return false;

    float u((a.x*sSeg.y - i.x*sSeg.y - sSeg.x*a.y + sSeg.x*i.y)/denom);
    float v((- sPos.x*a.y + sPos.x*i.y + sPos.y*a.x - sPos.y*i.x)/denom);
    if(u >= -1 && u <= 0 && v >= -1 && v <= 0)
    {
        if(out != nullptr)
            *out = a - sPos*u;
        return true;
    }
    return false;
}
