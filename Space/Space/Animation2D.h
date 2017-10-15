#pragma once

#include <Nazara/Math/Rect.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <vector>

struct Frame
{
	inline Frame(float _time = 0, const Nz::Rectui _texRectf = Nz::Rectui(0, 0, 1, 1), const Nz::Vector2f _offset = Nz::Vector2f(0, 0), bool _xFliped = false, bool _yFliped = false);

	float time;
	Nz::Rectui texRect;
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

	const Frame & getFrameAt(float time) const;
	Frame getFrameAt(float time);
	size_t getFrameIndexAt(float time) const;
	size_t frameCount() const;

	float toNormalizedTime(float time) const;

	inline Frame operator[](size_t index);
	inline const Frame & operator[](size_t index) const;

private:
	std::vector<Frame> m_frames;

	bool m_singleShoot;
	float m_totalTime;
};

#include "Animation2D.inl"

