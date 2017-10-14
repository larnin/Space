#include "Animation2D.h"

Frame::Frame(float _time, const Nz::Rectf _texRectf, const Nz::Vector2f _offset, bool _xFliped, bool _yFliped)
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