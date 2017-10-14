#pragma once

#include <Nazara/Math/Rect.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <vector>

struct Frame
{
	inline Frame(float _time = 0, const Nz::Rectf _texRectf = Nz::Rectf(0, 0, 1, 1), const Nz::Vector2f _offset = Nz::Vector2f(0, 0), bool _xFliped = false, bool _yFliped = false);

	Frame clone(float time) const;

	float time;
	Nz::Rectf texRect;
	Nz::Vector2f offset;
	bool xFliped;
	bool yFliped;
};

class Animation2D
{
public:
	Animation2D(bool singleShoot = false);
	~Animation2D() = default;

	void addFrame(const Frame & f);

	inline bool isSingleShoot() const;
	inline void setSingleShoot(bool value);
	inline float getTotalAnimationTime() const;

	Frame getFrameAt(float time) const;

private:
	std::vector<Frame> m_frames;

	bool m_singleShoot;
	float m_totalTime;
};

#include "Animation2D.inl"

