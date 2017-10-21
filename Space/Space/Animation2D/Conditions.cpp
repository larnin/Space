#include "Conditions.h"

bool AndCondition::check(const Animation2DEnv & env) const
{
	for(auto & c : *this)
	{
		if (!c->check(env))
			return false;
	}
	return true;
}

bool OrCondition::check(const Animation2DEnv & env) const
{
	for (auto & c : *this)
	{
		if (c->check(env))
			return true;
	}
	return false;
}

bool AnimationFinishedCondition::check(const Animation2DEnv & env) const
{
	return env.animationsFinished;
}

NotCondition::NotCondition(std::unique_ptr<ConditionBase> && _condition)
	: condition(std::move(_condition))
{
}

bool NotCondition::check(const Animation2DEnv & env) const
{
	return !condition->check(env);
}

XOrCondition::XOrCondition(std::unique_ptr<ConditionBase> && _condition1, std::unique_ptr<ConditionBase> && _condition2)
	: condition1(std::move(_condition1))
	, condition2(std::move(_condition2))
{
}

bool XOrCondition::check(const Animation2DEnv & env) const
{
	return condition1->check(env) != condition2->check(env);
}

PropertyConditionBase::PropertyConditionBase(const std::string & _key, int _value)
	: key(_key)
	, value(_value)
{
}


PropertyEqualCondition::PropertyEqualCondition(const std::string & _key, int _value)
	: PropertyConditionBase(_key, _value)
{
}

bool PropertyEqualCondition::check(const Animation2DEnv & env) const
{
	return value == env.propertyHolder.get(key);
}

PropertyNotEqualCondition::PropertyNotEqualCondition(const std::string & _key, int _value)
	: PropertyConditionBase(_key, _value)
{
}

bool PropertyNotEqualCondition::check(const Animation2DEnv & env) const
{
	return value != env.propertyHolder.get(key);
}

PropertySuperiorOrEqualCondition::PropertySuperiorOrEqualCondition(const std::string & _key, int _value)
	: PropertyConditionBase(_key, _value)
{
}

bool PropertySuperiorOrEqualCondition::check(const Animation2DEnv & env) const
{
	return value >= env.propertyHolder.get(key);
}

PropertyInferiorOrEqualCondition::PropertyInferiorOrEqualCondition(const std::string & _key, int _value)
	: PropertyConditionBase(_key, _value)
{
}

bool PropertyInferiorOrEqualCondition::check(const Animation2DEnv & env) const
{
	return value <= env.propertyHolder.get(key);
}

PropertySuperiorCondition::PropertySuperiorCondition(const std::string & _key, int _value)
	: PropertyConditionBase(_key, _value)
{
}

bool PropertySuperiorCondition::check(const Animation2DEnv & env) const
{
	return value > env.propertyHolder.get(key);
}

PropertyInferiorCondition::PropertyInferiorCondition(const std::string & _key, int _value)
	: PropertyConditionBase(_key, _value)
{
}

bool PropertyInferiorCondition::check(const Animation2DEnv & env) const
{
	return value < env.propertyHolder.get(key);
}

WaitCondition::WaitCondition(float _time)
	: time(_time)
{
}

bool WaitCondition::check(const Animation2DEnv & env) const
{
	return env.animationTime >= time;
}
