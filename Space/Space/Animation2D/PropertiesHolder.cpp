#include "PropertiesHolder.h"

void PropertiesHolder::set(const std::string & key, int value)
{
	m_properties[key] = value;
}

int PropertiesHolder::get(const std::string & key) const
{
	auto it = m_properties.find(key);
	if (it == m_properties.end())
		return 0;
	return it->second;
}

void PropertiesHolder::reset()
{
	m_properties.clear();
}
