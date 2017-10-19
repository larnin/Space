#include "Animation2DTransition.h"
#include <Animation2D/Animation2DState.h>

Animation2DTransition::Animation2DTransition(Animation2DState & dest)
	: m_dest(dest)
{

}

bool Animation2DTransition::check(const Animation2DEnv & env) const
{
	//todo
	return false;
}
