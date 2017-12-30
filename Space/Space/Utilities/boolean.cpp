#include "boolean.h"
#include "vect2ext.h"
#include <Nazara\Math\Rect.hpp>
#include <cassert>

#include <iostream>

struct ContactInfo
{
    ContactInfo(unsigned int _aIndex, unsigned int _bIndex, const Nz::Vector2f & _pos)
        : aIndex(_aIndex), bIndex(_bIndex), pos(_pos) {}
    unsigned int aIndex;
    unsigned int bIndex;
    Nz::Vector2f pos;
};

Nz::Vector2f centerShape(const Shape & s)
{
    assert(s.size() > 0 && "The shape must have a point");
    Nz::Vector2f center(0, 0);
    for(const auto & p : s)
        center += p;
    center /= float(s.size());
    return center;
}

Nz::Rectf aabbShape(const Shape & s)
{
    assert(s.size() > 0 && "The shape must have a point");
    Nz::Vector2f min = s[0];
	Nz::Vector2f max = s[0];
    for(const auto & p : s)
    {
        min = {std::min(p.x, min.x), std::min(p.y, min.y)};
        max = {std::max(p.x, max.x), std::max(p.y, max.y)};
    }
	return Nz::Rectf(min.x, min.y, max.x - min.x, max.y - min.y);
}

bool isPointOnShape(const Nz::Vector2f & p, const Shape & s, const Nz::Vector2f & shapeOffset)
{
    auto center = centerShape(s) + shapeOffset;
    auto aabb = aabbShape(s);

    auto dirCenter = center - p;
    auto endPos = p + dirCenter + normalize(dirCenter) * (aabb.width + aabb.height); //point out of the shape, but at the other side of p

    unsigned int intersectCount(0);
    for(unsigned int i(0) ; i < s.size() ; i++)
    {
        auto sP = (i > 0 ? s[i-1] : s.back()) + shapeOffset;
        if(interactSergment(p, endPos, sP, s[i] + shapeOffset))
            intersectCount ++;
    }

    if(intersectCount > 0)
        return intersectCount % 2 != 0;

    endPos = p - normalize(dirCenter) * (aabb.width + aabb.height); // same line but other direction
    for(unsigned int i(0) ; i < s.size() ; i++)
    {
        auto sP = (i > 0 ? s[i-1] : s.back()) + shapeOffset;
        if(interactSergment(p, endPos, sP, s[i] + shapeOffset))
            intersectCount ++;
    }
    return intersectCount % 2 != 0;
}

Shape offsetShape(Shape s, const Nz::Vector2f offset)
{
    for(auto & p : s)
        p += offset;
    return s;
}

unsigned int firstNotUsedContact(std::vector<unsigned int> contactsIndex)
{
    unsigned int bestIndex = 0;
    for(unsigned int i(0) ; i < contactsIndex.size() ; i++)
    {
        if(contactsIndex[i] == bestIndex)
        {
            i = 0;
            bestIndex++;
        }
    }
    return bestIndex;
}

void adaptShapesOrder(Shape & a, bool swapA, Shape & b, bool swapB, std::vector<ContactInfo> & contacts)
{
    if(swapA)
        switchPointsOrder(a);
    if(swapB)
        switchPointsOrder(b);

    for(auto & c : contacts)
    {
        if(swapA)
        {
            c.aIndex = a.size() - c.aIndex - 1;
            c.aIndex = c.aIndex <= 0 ? a.size() - 1 : c.aIndex - 1;
        }
        if(swapB)
        {
            c.bIndex = b.size() - c.bIndex - 1;
            c.bIndex = c.bIndex <= 0 ? b.size() - 1 : c.bIndex - 1;
        }
    }
}

std::vector<Shape> doBoolean(const Shape & a, const Shape & b, const Nz::Vector2f & offset, const std::vector<ContactInfo> & contacts)
{
    assert(!contacts.empty());

    std::vector<unsigned int> contactsIndexs;
    std::vector<Shape> shapes;

    while(contactsIndexs.size() < contacts.size())
    {
        unsigned int currentContactIndex(firstNotUsedContact(contactsIndexs));
        assert(currentContactIndex < contacts.size() && "Contact index must be smaller than contact size");
        unsigned int aIndex = contacts[currentContactIndex].aIndex;
        unsigned int bIndex = contacts[currentContactIndex].bIndex;
        bool isIn = isLeft(a[aIndex], aIndex >= a.size()-1 ? a[0] : a[aIndex + 1], b[bIndex] + offset);
        unsigned int startContactIndex = currentContactIndex;

        Shape s;
        contactsIndexs.push_back(currentContactIndex);
        do
        {
            if(isIn)
            {
                unsigned int nextContactIndex = currentContactIndex;
                unsigned int nextIndex = (bIndex + 1) % b.size();
                float maxDist = currentContactIndex >= contacts.size() ? norm(b[nextIndex] - b[bIndex]) :
                                        contacts[currentContactIndex].bIndex == bIndex ? norm(b[nextIndex] + offset - contacts[nextContactIndex].pos) :
                                                norm(b[nextIndex] - b[bIndex]);
                float currentDist = 0;
                for(unsigned int i(0) ; i < contacts.size() ; i++)
                {
                    if(contacts[i].bIndex != bIndex)
                        continue;
                    auto dist = norm(b[nextIndex] + offset - contacts[i].pos);
                    if(dist > currentDist && dist < maxDist)
                    {
                        currentDist = dist;
                        nextContactIndex = i;
                    }
                }
                if(nextContactIndex == currentContactIndex) //no new contact in the bIndex
                {
                    bIndex = nextIndex;
                    s.push_back(b[bIndex] + offset);
                    currentContactIndex = contacts.size();
                }
                else
                {
                    isIn = !isIn;
                    currentContactIndex = nextContactIndex;
                    contactsIndexs.push_back(currentContactIndex);
                    s.push_back(contacts[currentContactIndex].pos);
                    aIndex = contacts[currentContactIndex].aIndex;
                }
            }
            else
            {
                unsigned int nextContactIndex = currentContactIndex;
                unsigned int nextIndex = (aIndex + 1) % a.size();
                float maxDist = currentContactIndex >= contacts.size() ? norm(a[nextIndex] - a[bIndex]) :
										contacts[currentContactIndex].aIndex == aIndex ? norm(a[nextIndex] - contacts[nextContactIndex].pos) : 
												norm(a[nextIndex] - a[aIndex]);
                float currentDist = 0;
                for(unsigned int i(0) ; i < contacts.size() ; i++)
                {
                    if(contacts[i].aIndex != aIndex)
                        continue;
                    auto dist = norm(a[nextIndex] - contacts[i].pos);
                    if(dist > currentDist && dist < maxDist)
                    {
                        currentDist = dist;
                        nextContactIndex = i;
                    }
                }
                if(nextContactIndex == currentContactIndex) //no new contact in the aIndex
                {
                    aIndex = nextIndex;
                    s.push_back(a[aIndex]);
                    currentContactIndex = contacts.size();
                }
                else
                {
                    isIn = !isIn;
                    currentContactIndex = nextContactIndex;
                    contactsIndexs.push_back(currentContactIndex);
                    s.push_back(contacts[currentContactIndex].pos);
                    bIndex = contacts[currentContactIndex].bIndex;
                }
            }
        }while(currentContactIndex != startContactIndex);
        shapes.push_back(s);
    }

    return shapes;
}


std::vector<Shape> intersection(Shape a, Shape b, const Nz::Vector2f & offset, std::vector<ContactInfo> contacts)
{
    if(contacts.empty())
    {
        if(isPointOnShape(b[0], a, -offset)) //b in a
            return {b};
        if(isPointOnShape(a[0], b, offset)) //a in b
            return {a};
        return {}; //no intersection
    }

    adaptShapesOrder(a, !isClockWise(a), b, !isClockWise(b), contacts);

    return doBoolean(a, b, offset, contacts);
}

std::vector<Shape> add(Shape a, Shape b, const Nz::Vector2f & offset, std::vector<ContactInfo> contacts)
{
    if(contacts.empty())
    {
        if(isPointOnShape(b[0], a, -offset)) //b in a
            return {a};
        if(isPointOnShape(a[0], b, offset)) //a in b
            return {offsetShape(b, offset)};
        return {a, offsetShape(b, offset)};
    }

    adaptShapesOrder(a, isClockWise(a), b, isClockWise(b), contacts);

    auto shapes = doBoolean(a, b, offset, contacts);

    if(shapes.size() == 1)
        return shapes;

    unsigned int bestShape = 0;
	auto bestShapeSize = aabbShape(shapes[0]);
    for(unsigned int i(1) ; i < shapes.size() ; i++)
    {
        auto aabb = aabbShape(shapes[i]);
        if(aabb.width > bestShapeSize.width && aabb.height > bestShapeSize.height)
        {
            bestShapeSize = aabb;
            bestShape = i;
        }
    }

    return {shapes[bestShape]};
}

std::vector<Shape> substract(Shape a, Shape b, const Nz::Vector2f & offset, std::vector<ContactInfo> contacts)
{
    if(contacts.empty())
    {
        if(isPointOnShape(b[0], a, -offset)) //b in a
            return {a}; //shape with 2 surface, that is ignored
        if(isPointOnShape(a[0], b, offset)) //a in b
            return {};
        return {a};
    }

    adaptShapesOrder(a, !isClockWise(a), b, isClockWise(b), contacts);

    return doBoolean(a, b, offset, contacts);
}

Shape createCircularShape(float radius, unsigned int sides)
{
	const float step = 2 * M_PI / sides;
	Shape s;
	for (unsigned int i(0); i < sides; i++)
		s.push_back(radius * Nz::Vector2f(std::cos(step * i), std::sin(step * i)));
	return s;
}

std::vector<Shape> boolean(const Shape & a, const Shape & b, const Nz::Vector2f & offset, BooleanType type)
{
    assert((!isCrossed(a) && !isCrossed(b)) && "The shapes can't be crossed");

    std::vector<ContactInfo> intersections;
	Nz::Vector2f intersectionPos;
    for(unsigned int i(0) ; i < a.size() ; i++)
    {
        const auto & p1 = i < a.size() - 1 ? a[i+1] : a.front();
        const auto & p2 = a[i];
        for(unsigned int j(0) ; j < b.size() ; j++)
        {
            const auto & p3 = (j < b.size() - 1 ? b[j+1] : b.front()) + offset;
            const auto & p4 = b[j] + offset;
            if(interactSergment(p1, p2, p3, p4, &intersectionPos))
                intersections.emplace_back(i, j, intersectionPos);
        }
    }

    assert(intersections.size() % 2 == 0 && "Contact number is not pair");

    switch (type) {
    case BooleanType::SUBSTRACT:
        return substract(a, b, offset, intersections);
    case BooleanType::ADD:
        return add(a, b, offset, intersections);
    case BooleanType::INTERSECTION:
        return intersection(a, b, offset, intersections);
    default:
        assert(false && "Not supported boolean !");
    }

    return {};
}

bool isClockWise(const Shape & s)
{
    assert(!isCrossed(s) && "The shape can't be crossed");
    assert(s.size() >= 3 && "The shape must have at least 3 points");

    unsigned int topIndex(0);
    for(unsigned int i(1) ; i < s.size() ; i++)
        if(s[topIndex].y < s[i].y)
            topIndex = i;
    unsigned int leftIndex = topIndex > 0 ? topIndex - 1 : s.size() - 1;
    unsigned int rightIndex = topIndex < s.size() - 1 ? topIndex + 1 : 0;

    assert((isRight(s[leftIndex], s[topIndex], s[rightIndex]) || isLeft(s[leftIndex], s[topIndex], s[rightIndex])) && "Two faces of the shapes are collinear");

    return isRight(s[leftIndex], s[topIndex], s[rightIndex]);
}

Shape switchPointsOrder(const Shape & s)
{
    Shape s2;
    s2.reserve(s.size());
    for(auto it = s.rbegin() ; it != s.rend() ; it++)
        s2.push_back(*it);
    return s2;
}

void switchPointsOrder(Shape &s)
{
    for(unsigned int i = 0 ; i < s.size()/2 ; i++)
        std::swap(s[i], s[s.size() - i - 1]);
}

bool isCrossed(const Shape & s)
{
    if(s.size() <= 3) //a triangle can't be crossed
        return false;

    for(unsigned int i(0) ; i < s.size() ; i++)
    {
        const auto & p1 = i > 0 ? s[i-1] : s.back();
        const auto & p2 = s[i];
        for(unsigned int j(0) ; j < s.size() ; j++)
        {
            if(std::abs(int(i) - int(j)) <= 1 || (i == 0 && j == s.size() - 1) || (i == s.size() - 1 && j == 0))
                continue;
            if(interactSergment(p1, p2, j > 0 ? s[j-1] : s.back(), s[j]))
                return true;
        }
    }
    return false;
}

Nz::Vector2f nearestPointOn(const Nz::Vector2f & pos, const Shape & s)
{
	assert(s.size() >= 2 && "The shape must have at least 2 points");

	Nz::Vector2f bestPos = pos;
	float bestDistance = std::numeric_limits<float>::max();
	for (unsigned int i(0); i < s.size(); i++)
	{
		auto p1 = s[i];
		auto p2 = i == 0 ? s.back() : s[i - 1];

		auto d = dot(p2 - p1, pos - p1) / norm(p2 - p1);
		auto p = p1 + (p2 - p1) * d;
		if (d < 0)
			p = p1;
		if (d > 1)
			p = p2;

		float dist = normSqr(pos - p);
		if (dist < bestDistance)
		{
			bestDistance = dist;
			bestPos = p;
		}
	}
	return bestPos;
}
