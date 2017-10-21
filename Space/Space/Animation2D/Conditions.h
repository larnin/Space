#pragma once

#include "Animation2D/Animation2DEnv.h"
#include <vector>
#include <memory>

class ConditionBase
{
public:
	ConditionBase() = default;
	virtual ~ConditionBase() = default;
	
	virtual bool check(const Animation2DEnv & env) const = 0;
};

class ListConditionBase : public ConditionBase, private std::vector<std::unique_ptr<ConditionBase>>
{
public :
	ListConditionBase() = default;
	virtual ~ListConditionBase() = default;

	using std::vector<std::unique_ptr<ConditionBase>>::begin;
	using std::vector<std::unique_ptr<ConditionBase>>::end;
	using std::vector<std::unique_ptr<ConditionBase>>::size;
	using std::vector<std::unique_ptr<ConditionBase>>::operator[];
	using std::vector<std::unique_ptr<ConditionBase>>::clear;
	using std::vector<std::unique_ptr<ConditionBase>>::erase;
};

class AndCondition : public ListConditionBase
{
public:
	AndCondition() = default;
	~AndCondition() = default;

	bool check(const Animation2DEnv & env) const override;
};

class OrCondition : public ListConditionBase
{
public:
	OrCondition() = default;
	~OrCondition() = default;

	bool check(const Animation2DEnv & env) const override;
};

class NotCondition : public ConditionBase
{
public:
	NotCondition(std::unique_ptr<ConditionBase> && _condition);
	~NotCondition() = default;

	bool check(const Animation2DEnv & env) const override;

	std::unique_ptr<ConditionBase> condition;
};

class XOrCondition : public ConditionBase
{
public:
	XOrCondition(std::unique_ptr<ConditionBase> && _condition1, std::unique_ptr<ConditionBase> && _condition2);
	~XOrCondition() = default;

	bool check(const Animation2DEnv & env) const override;

	std::unique_ptr<ConditionBase> condition1;
	std::unique_ptr<ConditionBase> condition2;
};

class AnimationFinishedCondition : public ConditionBase
{
public:
	AnimationFinishedCondition() = default;
	~AnimationFinishedCondition() = default;
	
	bool check(const Animation2DEnv & env) const override;
};

class PropertyConditionBase : public ConditionBase
{
public:
	PropertyConditionBase(const std::string & _key, int _value);
	virtual ~PropertyConditionBase() = default;

	std::string key;
	int value;
};

class PropertyEqualCondition : public PropertyConditionBase
{
public:
	PropertyEqualCondition(const std::string & _key, int _value);
	~PropertyEqualCondition() = default;

	bool check(const Animation2DEnv & env) const override;
};

class PropertyNotEqualCondition : public PropertyConditionBase
{
public:
	PropertyNotEqualCondition(const std::string & _key, int _value);
	~PropertyNotEqualCondition() = default;

	bool check(const Animation2DEnv & env) const override;
};

class PropertySuperiorOrEqualCondition : public PropertyConditionBase
{
public:
	PropertySuperiorOrEqualCondition(const std::string & _key, int _value);
	~PropertySuperiorOrEqualCondition() = default;

	bool check(const Animation2DEnv & env) const override;
};

class PropertyInferiorOrEqualCondition : public PropertyConditionBase
{
public:
	PropertyInferiorOrEqualCondition(const std::string & _key, int _value);
	~PropertyInferiorOrEqualCondition() = default;

	bool check(const Animation2DEnv & env) const override;
};

class PropertySuperiorCondition : public PropertyConditionBase
{
public:
	PropertySuperiorCondition(const std::string & _key, int _value);
	~PropertySuperiorCondition() = default;

	bool check(const Animation2DEnv & env) const override;
};

class PropertyInferiorCondition : public PropertyConditionBase
{
public:
	PropertyInferiorCondition(const std::string & _key, int _value);
	~PropertyInferiorCondition() = default;

	bool check(const Animation2DEnv & env) const override;
};

class WaitCondition : public ConditionBase
{
public:
	WaitCondition(float _time);
	~WaitCondition() = default;

	bool check(const Animation2DEnv & env) const override;

	float time;
};