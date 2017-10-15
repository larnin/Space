#include "Animation2D/Animation2D.h"
#include <limits>

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

size_t Animation2D::getFrameIndexAt(float time) const
{
	NazaraAssert(m_frames.size() > 0, "There are no frame in the animation !");

	time = toNormalizedTime(time);

	float current = 0;
	for (size_t i(0) ; i < m_frames.size() ; i++)
	{
		const auto & f = m_frames[i];
		current += f.time;
		if (current >= time)
			return i;
	}

	return m_frames.size() - 1;
}

size_t Animation2D::frameCount() const
{
	return m_frames.size();
}

const Frame & Animation2D::getFrameAt(float time) const
{
	return m_frames[getFrameIndexAt(time)];
}

Frame Animation2D::getFrameAt(float time)
{
	return m_frames[getFrameIndexAt(time)];
}

float Animation2D::toNormalizedTime(float time) const
{
	if (m_singleShoot && time < 0)
		return 0;
	if (m_singleShoot && time > m_totalTime)
		return m_totalTime;

	if (time >= 0)
		return fmod(time, m_totalTime);
	return m_totalTime + fmod(time, m_totalTime);
}



