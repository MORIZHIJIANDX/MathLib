#pragma once

#include "../math/MathType.h"
#include "../math/Transform.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace Dash
{
	struct HitInfo
	{
		Vector3f Position;
		Vector3f Normal;
		Vector3f Tangent;
		Vector2f TexCoord;
	};

	struct InputLayoutElement
	{
		std::string SemanticName;
		std::size_t SemanticIndex;
		DASH_FORMAT Format;
		std::size_t AlignedByteOffset;

		InputLayoutElement(const std::string& name, std::size_t index, DASH_FORMAT format, std::size_t offset)
			: SemanticName(name)
			, SemanticIndex(index)
			, Format(format)
			, AlignedByteOffset(offset)
		{
		}
	};

	struct MeshPart
	{
		std::size_t VertexStart;
		std::size_t VertexCount;
		std::size_t IndexStart;
		std::size_t IndexCount;
		std::size_t MaterialIdx;

		MeshPart(std::size_t vertexStart = 0, std::size_t vertexCount = 0,
			std::size_t indexStart = 0, std::size_t indexCount = 0,
			std::size_t materialIndex = 0)
			: VertexStart(vertexStart), VertexCount(vertexCount)
			, IndexStart(indexStart), IndexCount(indexCount), MaterialIdx(materialIndex)
		{
		}
	};

	struct TriangleMesh
	{	
		void GetVertexPosition(Vector3f& p, std::size_t vertexIndex)
		{
			GetVertexProperty("POSITION", vertexIndex, p);
		}

		void GetVertexNormal(Vector3f& p, std::size_t vertexIndex)
		{
			GetVertexProperty("NORMAL", vertexIndex, p);
		}

		void GetVertexTangent(Vector3f& p, std::size_t vertexIndex)
		{
			GetVertexProperty("TANGENT", vertexIndex, p);
		}

		void GetVertexTexCoord(Vector2f& p, std::size_t vertexIndex)
		{
			GetVertexProperty("TEXCOORD", vertexIndex, p);
		}

		template<typename T>
		void GetVertexProperty(const std::string& name, std::size_t vertexIndex, T& value)
		{
			std::size_t propertyOffset = GetVertexPropertyOffset(name, vertexIndex);
			GetData(value, Vertices.data(), propertyOffset);
		}

		std::size_t GetVertexPropertyOffset(const std::string& name, std::size_t vertexIndex)
		{
			auto iter = InputElementMap.find(name);
			ASSERT(iter != InputElementMap.end());

			return VertexStride * vertexIndex + iter->second;
		}

		std::unordered_map<std::string, std::size_t> InputElementMap; // pair{ SemanticName, Offset }
		std::vector<InputLayoutElement> InputElements;
		std::vector<MeshPart> MeshParts;
		std::size_t VertexStride = 0;
		std::size_t NumVertices = 0;
		std::size_t NumIndices = 0;
		DASH_FORMAT IndexType = DASH_FORMAT::R32_UINT;
		std::vector<std::uint8_t> Vertices;
		std::vector<std::uint8_t> Indices;
	}; 

	class Shape
	{
	public:
		Shape(const Transform& objectToWorld, const Transform& worldToObject) noexcept;
		~Shape() = default;

		virtual bool Intersection(const Ray& r, Scalar* t, HitInfo* hitInfo) const noexcept = 0;
		virtual bool IntersectionFast(const Ray& r) const noexcept
		{
			return Intersection(r, nullptr, nullptr);
		}

		virtual BoundingBox ObjectBound() const noexcept = 0;
		virtual BoundingBox WorldBound() const noexcept;

		virtual std::shared_ptr<TriangleMesh> ConvertToTriangleMesh() const noexcept  = 0;
	
		const Transform& ObjectToWorld;
		const Transform& WorldToObject;
	};
}