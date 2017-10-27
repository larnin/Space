#pragma once

#include "Animation2D/Animation2D.h"
#include "Animation2D/Animation2DState.h"
#include <Nazara/Core/RefCounted.hpp>
#include <Nazara/Core/ObjectLibrary.hpp>
#include <vector>
#include <list>

class Animator2D;

using Animator2DConstRef = Nz::ObjectRef<const Animator2D>;
using Animator2DLibrary = Nz::ObjectLibrary<Animator2D>;
using Animator2DRef = Nz::ObjectRef<Animator2D>;

class Animator2D : public Nz::RefCounted
{
public:
	Animator2D(Animation2DRef animation);
	Animator2D() = default;
	~Animator2D() = default;
	Animator2D(Animator2D &&) = default;
	Animator2D & operator=(Animator2D &&) = default;

	void addAnimation(Animation2DRef animation);
	void removeAnimation(Animation2DRef animation);
	void removeAnimation(const std::string & name);
	Animation2DRef getAnimation(const std::string & name) const;
	bool animationExist(const std::string & name) const;
	size_t getAnimationCount() const;
	Animation2DRef getAnimation(size_t index) const;

	Animation2DState & addState(Animation2DState && state);
	void removeState(const Animation2DState & state);
	void removeState(const std::string & name);
	Animation2DState const & getState(const std::string & name) const;
	Animation2DState & getState(const std::string & name);
	bool stateExist(const std::string & name) const;

	void setDefaultStateName(const std::string & name);
	std::string getDefaultStateName() const;

	template<typename... Args> Animation2DState & addState(Args&&... args)
	{
		return addState(Animation2DState(std::forward<Args>(args)...));
	}

	template<typename... Args> static Animator2DRef New(Args&&... args)
	{
		std::unique_ptr<Animator2D> object(new Animator2D(std::forward<Args>(args)...));
		object->SetPersistent(false);

		return object.release();
	}

private:
	std::vector<Animation2DRef> m_animations;
	std::list<Animation2DState> m_states;

	std::string m_defaultStateName;
};

