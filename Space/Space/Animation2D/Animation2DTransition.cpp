#include "Animation2DTransition.h"
#include <Animation2D/Animation2DState.h>

Animation2DTransition::Animation2DTransition(Animation2DState & dest, std::unique_ptr<ConditionBase> && condition)
	: m_dest(dest)
	, m_condition(std::move(condition))
{

}

bool Animation2DTransition::check(const Animation2DEnv & env) const
{
	return m_condition->check(env);
}
