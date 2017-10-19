#include "Animation2DPropertiesHolder.h"

void Animation2DPropertiesHolder::set(const std::string & key, int value)
{
	m_properties[key] = value;
}

int Animation2DPropertiesHolder::get(const std::string & key) const
{
	auto it = m_properties.find(key);
	if (it == m_properties.end())
		return 0;
	return it->second;
}
