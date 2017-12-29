#pragma once

#include <Nazara/Utility/Image.hpp>
#include <Nazara/Core/RefCounted.hpp>
#include <vector>
#include <map>
#include <string>

#include "Matrix.h"

using AsteroidData = Matrix<float>;
using Shape = std::vector<Nz::Vector2f>;

class AsteroidBaseInfos;

using AsteroidBaseInfosConstRef = Nz::ObjectRef<const AsteroidBaseInfos>;
using AsteroidBaseInfosRef = Nz::ObjectRef<AsteroidBaseInfos>;

class AsteroidBaseInfos : public Nz::RefCounted
{
public:
	AsteroidBaseInfos(const Nz::ImageRef & _asteroid, const Nz::ImageRef & _explosion, const Nz::ImageRef & _border, const Shape & _shape);
	~AsteroidBaseInfos() = default;

	template<typename... Args>
	static AsteroidBaseInfosRef New(Args&&... args)
	{
		auto object = std::make_unique<AsteroidBaseInfos>(std::forward<Args>(args)...);
		object->SetPersistent(false);

		return object.release();
	}

	static void createLibrary();

	static void registerInfos(const std::string & name, std::string & asteroidName, const std::string & explosionName, const std::string & borderName, const Shape & shape);
	static AsteroidBaseInfosRef getAsteroidInfos(const std::string & name);
	static bool asteroidInfosExist(const std::string & name);

	static void registerImage(const std::string & imageName, const std::string & imagePath);
	static Nz::ImageRef getImage(const std::string & imageName);
	static bool imageExist(const std::string & imageName);

	Nz::ImageRef asteroid;
	Nz::ImageRef explosion;
	Nz::ImageRef border;
	Shape shape;
	AsteroidData data;

private:
	static std::map<std::string, Nz::ImageRef> m_images;
	static std::map<std::string, AsteroidBaseInfosRef> m_infos;
};

