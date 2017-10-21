#pragma once

#include "Animation2D/Animation2D.h"
#include "Animation2D/Animation2DState.h"
#include <Nazara/Core/RefCounted.hpp>
#include <Nazara/Core/ObjectLibrary.hpp>
#include <vector>

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

	//todo add/remove anim/state


	template<typename... Args> static Animator2DRef New(Args&&... args)
	{
		std::unique_ptr<Animator2D> object(new Animator2D(std::forward<Args>(args)...));
		object->SetPersistent(false);

		return object.release();
	}

private:
	std::vector<Animation2DRef> m_animations;
	std::vector<Animation2DState> m_states;
};

