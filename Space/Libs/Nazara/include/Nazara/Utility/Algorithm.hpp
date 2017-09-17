﻿// Copyright (C) 2017 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_ALGORITHM_UTILITY_HPP
#define NAZARA_ALGORITHM_UTILITY_HPP

#include <Nazara/Prerequesites.hpp>
#include <Nazara/Core/Color.hpp>
#include <Nazara/Core/SparsePtr.hpp>
#include <Nazara/Math/Box.hpp>
#include <Nazara/Math/Matrix4.hpp>
#include <Nazara/Math/Quaternion.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Math/Vector3.hpp>
#include <Nazara/Math/Vector4.hpp>
#include <Nazara/Utility/IndexIterator.hpp>
#include <Nazara/Utility/Mesh.hpp>
#include <Nazara/Utility/SkeletalMesh.hpp>

namespace Nz
{
	struct SkinningData
	{
		const Joint* joints;
		const SkeletalMeshVertex* inputVertex;
		MeshVertex* outputVertex;
	};

	struct VertexPointers
	{
		SparsePtr<Vector3f> normalPtr;
		SparsePtr<Vector3f> positionPtr;
		SparsePtr<Vector3f> tangentPtr;
		SparsePtr<Vector2f> uvPtr;
	};

	NAZARA_UTILITY_API Boxf ComputeAABB(SparsePtr<const Vector3f> positionPtr, unsigned int vertexCount);
	NAZARA_UTILITY_API void ComputeBoxIndexVertexCount(const Vector3ui& subdivision, unsigned int* indexCount, unsigned int* vertexCount);
	NAZARA_UTILITY_API unsigned int ComputeCacheMissCount(IndexIterator indices, unsigned int indexCount);
	NAZARA_UTILITY_API void ComputeConeIndexVertexCount(unsigned int subdivision, unsigned int* indexCount, unsigned int* vertexCount);
	NAZARA_UTILITY_API void ComputeCubicSphereIndexVertexCount(unsigned int subdivision, unsigned int* indexCount, unsigned int* vertexCount);
	NAZARA_UTILITY_API void ComputeIcoSphereIndexVertexCount(unsigned int recursionLevel, unsigned int* indexCount, unsigned int* vertexCount);
	NAZARA_UTILITY_API void ComputePlaneIndexVertexCount(const Vector2ui& subdivision, unsigned int* indexCount, unsigned int* vertexCount);
	NAZARA_UTILITY_API void ComputeUvSphereIndexVertexCount(unsigned int sliceCount, unsigned int stackCount, unsigned int* indexCount, unsigned int* vertexCount);

	NAZARA_UTILITY_API void GenerateBox(const Vector3f& lengths, const Vector3ui& subdivision, const Matrix4f& matrix, const Rectf& textureCoords, VertexPointers vertexPointers, IndexIterator indices, Boxf* aabb = nullptr, unsigned int indexOffset = 0);
	NAZARA_UTILITY_API void GenerateCone(float length, float radius, unsigned int subdivision, const Matrix4f& matrix, const Rectf& textureCoords, VertexPointers vertexPointers, IndexIterator indices, Boxf* aabb = nullptr, unsigned int indexOffset = 0);
	NAZARA_UTILITY_API void GenerateCubicSphere(float size, unsigned int subdivision, const Matrix4f& matrix, const Rectf& textureCoords, VertexPointers vertexPointers, IndexIterator indices, Boxf* aabb = nullptr, unsigned int indexOffset = 0);
	NAZARA_UTILITY_API void GenerateIcoSphere(float size, unsigned int recursionLevel, const Matrix4f& matrix, const Rectf& textureCoords, VertexPointers vertexPointers, IndexIterator indices, Boxf* aabb = nullptr, unsigned int indexOffset = 0);
	NAZARA_UTILITY_API void GeneratePlane(const Vector2ui& subdivision, const Vector2f& size, const Matrix4f& matrix, const Rectf& textureCoords, VertexPointers vertexPointers, IndexIterator indices, Boxf* aabb = nullptr, unsigned int indexOffset = 0);
	NAZARA_UTILITY_API void GenerateUvSphere(float size, unsigned int sliceCount, unsigned int stackCount, const Matrix4f& matrix, const Rectf& textureCoords, VertexPointers vertexPointers, IndexIterator indices, Boxf* aabb = nullptr, unsigned int indexOffset = 0);

	NAZARA_UTILITY_API void OptimizeIndices(IndexIterator indices, unsigned int indexCount);

	NAZARA_UTILITY_API void SkinPosition(const SkinningData& data, unsigned int startVertex, unsigned int vertexCount);
	NAZARA_UTILITY_API void SkinPositionNormal(const SkinningData& data, unsigned int startVertex, unsigned int vertexCount);
	NAZARA_UTILITY_API void SkinPositionNormalTangent(const SkinningData& data, unsigned int startVertex, unsigned int vertexCount);

	NAZARA_UTILITY_API void TransformVertices(VertexPointers vertexPointers, unsigned int vertexCount, const Matrix4f& matrix);
	
	template<typename T>
	struct IsSuitableForComponent
	{
		constexpr static bool value = false;
	};

	template<class T> constexpr const ComponentType componentTypeId() 
	{ 
		static_assert(IsSuitableForComponent<T>::value, "This type cannot be used as a component.");
		return ComponentType{}; 
	}

	template<> constexpr const ComponentType componentTypeId<Color>() { return ComponentType_Color; }
	template<> constexpr const ComponentType componentTypeId<double>() { return ComponentType_Double1; }
	template<> constexpr const ComponentType componentTypeId<Vector2d>() { return ComponentType_Double2; }
	template<> constexpr const ComponentType componentTypeId<Vector3d>() { return ComponentType_Double3; }
	template<> constexpr const ComponentType componentTypeId<Vector4d>() { return ComponentType_Double4; }
	template<> constexpr const ComponentType componentTypeId<float>() { return ComponentType_Float1; }
	template<> constexpr const ComponentType componentTypeId<Vector2f>() { return ComponentType_Float2; }
	template<> constexpr const ComponentType componentTypeId<Vector3f>() { return ComponentType_Float3; }
	template<> constexpr const ComponentType componentTypeId<Vector4f>() { return ComponentType_Float4; }
	template<> constexpr const ComponentType componentTypeId<int>() { return ComponentType_Int1; }
	template<> constexpr const ComponentType componentTypeId<Vector2i>() { return ComponentType_Int2; }
	template<> constexpr const ComponentType componentTypeId<Vector3i>() { return ComponentType_Int3; }
	template<> constexpr const ComponentType componentTypeId<Vector4i>() { return ComponentType_Int4; }
	template<> constexpr const ComponentType componentTypeId<Quaternionf>() { return ComponentType_Quaternion; }

	template<typename T>
	constexpr const ComponentType GetComponentTypeOf() { return componentTypeId<std::decay_t<T>>(); }

	/*template <typename T, std::enable_if_t<std::is_same<std::remove_const_t<T>, Color>::value>...>
	ComponentType GetComponentTypeOf() { return ComponentType_Color; }

	template <typename T, std::enable_if_t<std::is_same<std::remove_const_t<T>, double>::value>...>
	ComponentType GetComponentTypeOf() { return ComponentType_Double1; }

	template <typename T, std::enable_if_t<std::is_same<std::remove_const_t<T>, Vector2d>::value>...>
	ComponentType GetComponentTypeOf() { return ComponentType_Double2; }

	template <typename T, std::enable_if_t<std::is_same<std::remove_const_t<T>, Vector3d>::value>...>
	ComponentType GetComponentTypeOf() { return ComponentType_Double3; }

	template <typename T, std::enable_if_t<std::is_same<std::remove_const_t<T>, Vector4d>::value>...>
	ComponentType GetComponentTypeOf() { return ComponentType_Double4; }

	template <typename T, std::enable_if_t<std::is_same<std::remove_const_t<T>, float>::value>...>
	ComponentType GetComponentTypeOf() { return ComponentType_Float1; }

	template <typename T, std::enable_if_t<std::is_same<std::remove_const_t<T>, Vector2f>::value>...>
	ComponentType GetComponentTypeOf() { return ComponentType_Float2; }

	template <typename T, std::enable_if_t<std::is_same<std::remove_const_t<T>, Vector3f>::value>...>
	ComponentType GetComponentTypeOf() { return ComponentType_Float3; }

	template <typename T, std::enable_if_t<std::is_same<std::remove_const_t<T>, Vector4f>::value>...>
	ComponentType GetComponentTypeOf() { return ComponentType_Float4; }

	template <typename T, std::enable_if_t<std::is_same<std::remove_const_t<T>, int>::value>...>
	ComponentType GetComponentTypeOf() { return ComponentType_Int1; }

	template <typename T, std::enable_if_t<std::is_same<std::remove_const_t<T>, Vector2i>::value>...>
	ComponentType GetComponentTypeOf() { return ComponentType_Int2; }

	template <typename T, std::enable_if_t<std::is_same<std::remove_const_t<T>, Vector3i>::value>...>
	ComponentType GetComponentTypeOf() { return ComponentType_Int3; }

	template <typename T, std::enable_if_t<std::is_same<std::remove_const_t<T>, Vector4i>::value>...>
	ComponentType GetComponentTypeOf() { return ComponentType_Int4; }

	template <typename T, std::enable_if_t<std::is_same<std::remove_const_t<T>, Quaternionf>::value>...>
	ComponentType GetComponentTypeOf() { return ComponentType_Quaternion; }*/
}

#endif // NAZARA_ALGORITHM_UTILITY_HPP
