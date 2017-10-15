#include "Animation2D/Animation2D.h"

Frame::Frame(float _time, const Nz::Rectui _texRectf, const Nz::Vector2f _offset, bool _xFliped, bool _yFliped)
	: time(_time)
	, texRect(_texRectf)
	, offset(_offset)
	, xFliped(_xFliped)
	, yFliped(_yFliped)
{

}

inline bool Animation2D::isSingleShoot() const
{
	return m_singleShoot;
}

inline void Animation2D::setSingleShoot(bool value)
{
	m_singleShoot = value;
}

inline float Animation2D::getTotalAnimationTime() const
{
	return m_totalTime;
}

inline Frame Animation2D::operator[](size_t index)
{
	return m_frames[index];
}

inline const Frame & Animation2D::operator[](size_t index) const
{
	return m_frames[index];
}