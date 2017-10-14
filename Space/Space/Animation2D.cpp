#include "Animation2D.h"
#include <limits>

Frame Frame::clone(float time) const
{
	Frame f = *this;
	f.time = time;
	return f;
}

Animation2D::Animation2D(bool singleShoot)
	: m_singleShoot(singleShoot)
	, m_totalTime(0)
{
}

void Animation2D::addFrame(const Frame & f)
{
	m_frames.push_back(f);
	m_totalTime += f.time;
}

Frame Animation2D::getFrameAt(float time) const
{
	NazaraAssert(m_frames.size() > 0, "There are no frame in the animation !");

	if (m_singleShoot && time > m_totalTime)
		return m_frames.back().clone(std::numeric_limits<float>::max());
		
	time = fmod(time, m_totalTime);

	float current = 0;
	for (const auto f : m_frames)
	{
		current += f.time;
		if (current >= time)
		{
			return f.clone(current - time);
		}
	}

	return m_frames.back();
}

