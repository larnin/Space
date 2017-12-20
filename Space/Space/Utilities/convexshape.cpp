#include "convexshape.h"
#include "boolean.h"
#include "vect2ext.h"
#include <limits>
#include <cassert>

#include <iostream>

bool isConvexe(const Shape & s)
{
    if(s.size() <= 3) //a triangle is convexe
        return true;

    bool turnLeft = false;
    bool turnRight = false;

    for(unsigned int i(0) ; i < s.size() ; i++)
    {
        unsigned int i1 = (i+1) % s.size();
        unsigned int i2 = (i+2) % s.size();

        if(isLeft(s[i], s[i1], s[i2]))
            turnLeft = true;
        else if(isRight(s[i], s[i1], s[i2]))
            turnRight = true;
        if(turnLeft && turnRight)
            return false;
    }
    return true;
}

bool isValidPoint(const Shape & s, unsigned int origineID, unsigned int testedID) // need s defined clockwise
{
    if(origineID == testedID)
        return false;
    if(origineID == testedID - 1 || origineID == testedID + 1)
        return false;
    if(origineID == 0 && testedID == s.size() - 1)
        return false;
    if(origineID == s.size() - 1 && testedID == 0)
        return false;

    auto leftP = origineID > 0 ? origineID - 1 : s.size() - 1;
    auto rightP = origineID < s.size() - 1 ? origineID + 1 : 0;

    if(isRight(s[origineID], s[leftP], s[testedID]) || isLeft(s[origineID], s[rightP], s[testedID]))
        return false;

    auto p1 = s[origineID];
    auto p2 = s[testedID];
    for(unsigned int i(0) ; i < s.size() ; i++)
    {
        unsigned int i2 = (i + 1)%s.size();

        if(i == origineID || i == testedID || i2 == origineID || i2 == testedID)
            continue;
        if(interactSergment(p1, p2, s[i], s[i2]))
            return false;
    }

    return true;
}

std::vector<Shape> makeConvexe(const Shape & s)
{
    if(!isClockWise(s))
        return makeConvexe(switchPointsOrder(s));

    std::vector<unsigned int> nonConvexeIndexs;

    for(unsigned int i(0) ; i < s.size() ; i++)
    {
        if(isLeft(s[i], s[(i+1) % s.size()], s[(i+2) % s.size()]))
            nonConvexeIndexs.push_back((i+1) % s.size());
    }
    if(nonConvexeIndexs.size() == 0)
        return {s};
    auto id = nonConvexeIndexs[0];
    auto left = s[id] - s[id >= s.size() - 1 ? 0 : id + 1];
    auto right = s[id] - s[id <= 0 ? s.size() - 1 : id - 1];
    auto dir = normalize(left) + normalize(right); //normale of the dir
    unsigned int bestNonConvexeIndex(0);
    float bestAngle = std::numeric_limits<float>::max();

    for(unsigned int i(1) ; i < nonConvexeIndexs.size() ; i++)
    {
        if(!isValidPoint(s, id, nonConvexeIndexs[i]))
            continue;

        float a = angle(dir, s[nonConvexeIndexs[i]] - s[id]);
        if(a < bestAngle)
        {
            bestAngle = a;
            bestNonConvexeIndex = i;
        }
    }

    if(bestNonConvexeIndex == 0)
    {
        bestAngle = std::numeric_limits<float>::max();
        for(unsigned int i(0) ; i < s.size() ; i++)
        {
            if(!isValidPoint(s, id, i))
                continue;

            float a = angle(dir, s[i] - s[id]);
            if(a < bestAngle)
            {
                bestAngle = a;
                bestNonConvexeIndex = i;
            }
        }
    }
    else
        bestNonConvexeIndex = nonConvexeIndexs[bestNonConvexeIndex];

    unsigned int id1 = std::min(id, bestNonConvexeIndex);
    unsigned int id2 = std::max(id, bestNonConvexeIndex);

    Shape s1, s2;
    for(unsigned int i(0) ; i < s.size() ; i++)
    {
        if(i == id1 || i == id2)
        {
            s1.push_back(s[i]);
            s2.push_back(s[i]);
        }
        else if(i < id1 || i > id2)
            s1.push_back(s[i]);
        else s2.push_back(s[i]);
    }

    //return {s1, s2};

    assert(s1.size() > 2 && s2.size() > 2 && "shapes must have at least 3 points");

    auto shapes1 = makeConvexe(s1);
    auto shapes2 = makeConvexe(s2);
    shapes1.insert(shapes1.end(), shapes2.begin(), shapes2.end());
    return shapes1;
}
