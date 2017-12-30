#include "AsteroidBaseInfos.h"
#include "asteroidrender.h"
#include <cassert>

std::map<std::string, Nz::ImageRef> AsteroidBaseInfos::m_images;
std::map<std::string, AsteroidBaseInfosRef> AsteroidBaseInfos::m_infos;

AsteroidBaseInfos::AsteroidBaseInfos(const Nz::ImageRef & _asteroid, const Nz::ImageRef & _explosion, const Nz::ImageRef & _border, const Shape & _shape)
	: asteroid(_asteroid)
	, explosion(_explosion)
	, border(_border)
	, shape(_shape)
	, data(createAsteroidData(*asteroid))
{

}

AsteroidBaseInfos::AsteroidBaseInfos(const AsteroidBaseInfos & base)
	: asteroid(base.asteroid)
	, explosion(base.explosion)
	, border(base.border)
	, shape(base.shape)
	, data(base.data)
{

}

void AsteroidBaseInfos::createLibrary()
{
	//todo fill library
}

void AsteroidBaseInfos::registerInfos(const std::string & name, const std::string & asteroidName, const std::string & explosionName, const std::string & borderName, const Shape & shape)
{
	auto it(m_infos.find(name));
	assert(it == m_infos.end() && "An image is already registered in that index");

	m_infos.emplace(name, AsteroidBaseInfos::New(getImage(asteroidName), getImage(explosionName), getImage(borderName), shape));
}

AsteroidBaseInfosRef AsteroidBaseInfos::getAsteroidInfos(const std::string & name)
{
	auto it(m_infos.find(name));
	assert(it != m_infos.end() && "The image don't exist");
	return it->second;
}

bool AsteroidBaseInfos::asteroidInfosExist(const std::string & name)
{
	auto it(m_infos.find(name));
	return it != m_infos.end();
}

void AsteroidBaseInfos::registerImage(const std::string & imageName, const std::string & imagePath)
{
	auto it(m_images.find(imageName));
	assert(it == m_images.end() && "An image is already registered in that index");
	Nz::ImageParams params;
	params.loadFormat = Nz::PixelFormatType::PixelFormatType_RGBA8;
	auto image = Nz::Image::New();
	image->LoadFromFile(imagePath, params);
	m_images.emplace(imageName, image);
}

Nz::ImageRef AsteroidBaseInfos::getImage(const std::string & imageName)
{
	auto it(m_images.find(imageName));
	assert(it != m_images.end() && "The image don't exist");
	return it->second;
}

bool AsteroidBaseInfos::imageExist(const std::string & imageName)
{
	auto it(m_images.find(imageName));
	return it != m_images.end();
}

