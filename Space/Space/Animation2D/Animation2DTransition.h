#pragma once

#include "Animation2D/Animation2DEnv.h"
#include "Animation2D/Conditions.h"
#include <memory>

class Animation2DState;

class Animation2DTransition
{
public:
	Animation2DTransition(Animation2DState & dest, std::unique_ptr<ConditionBase> && condition);
	~Animation2DTransition() = default;

	inline Animation2DState & getDest() const { return m_dest; }

	bool check(const Animation2DEnv & env) const;

private:
	Animation2DState & m_dest;
	std::unique_ptr<ConditionBase> m_condition;
};

