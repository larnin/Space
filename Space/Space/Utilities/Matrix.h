#pragma once

#include <cassert>
#include <vector>

template <typename T>
class Matrix
{
public:
	Matrix(unsigned int width, unsigned int height, T defaultValue = T())
		: m_datas(width*height, defaultValue)
		, m_width(width)
		, m_height(height)
	{}

	typename std::vector<T>::reference operator()(unsigned int x, unsigned int y)
	{
		assert(x < m_width && y < m_height);
		return m_datas[x + y * m_width];
	}

	typename std::vector<T>::const_reference operator ()(unsigned int x, unsigned int y) const
	{
		assert(x < m_width && y < m_height);
		return m_datas[x + y * m_width];
	}

	unsigned int width() const
	{
		return m_width;
	}

	unsigned int height() const
	{
		return m_height;
	}

	typename std::vector<T>::iterator begin()
	{
		return m_datas.begin();
	}

	typename std::vector<T>::iterator end()
	{
		return m_datas.end();
	}

	typename std::vector<T>::const_iterator begin() const
	{
		return m_datas.begin();
	}

	typename std::vector<T>::const_iterator end() const
	{
		return m_datas.end();
	}

private:
	std::vector<T> m_datas;
	unsigned int m_width;
	unsigned int m_height;

};

