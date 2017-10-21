#pragma once

#include <unordered_map>
#include <string>

class PropertiesHolder
{
public:
	PropertiesHolder() = default;
	~PropertiesHolder() = default;

	void set(const std::string & key, int value = 0);
	int get(const std::string & key) const;

private:
	std::unordered_map<std::string, int> m_properties;
};

