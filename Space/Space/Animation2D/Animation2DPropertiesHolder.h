#pragma once

#include <unordered_map>
#include <string>

class Animation2DPropertiesHolder
{
public:
	Animation2DPropertiesHolder() = default;
	~Animation2DPropertiesHolder() = default;

	void set(const std::string & key, int value = 0);
	int get(const std::string & key) const;

private:
	std::unordered_map<std::string, int> m_properties;
};

