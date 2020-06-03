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
		Math::Vector3f Position;
		Math::Vector3f Normal;
		Math::Vector3f Tangent;
		Math::Vector2f TexCoord;
	};

	enum class DASH_FORMAT
	{
		R16_UINT,
		R32_UINT,
		R32_FLOAT,
		R32G32_FLOAT,
		R32G32B32_FLOAT,
		R32G32B32A32_FLOAT,
	};


	FORCEINLINE std::size_t GetDashFormatSize(DASH_FORMAT format)
	{
		switch (format)
		{
		case DASH_FORMAT::R16_UINT:
			return 2;
			break;
		case DASH_FORMAT::R32_UINT:
		case DASH_FORMAT::R32_FLOAT:
			return 4;
			break;
		case DASH_FORMAT::R32G32_FLOAT:
			return 8;
			break;
		case DASH_FORMAT::R32G32B32_FLOAT:
			return 12;
			break;
		case DASH_FORMAT::R32G32B32A32_FLOAT:
			return 16;
			break;
		default:
			ASSERT(false);
			break;
		}

		return 0;
	}

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
		void GetVertexPosition(Math::Vector3f& p, std::size_t vertexIndex)
		{
			GetVertexProperty("POSITION", vertexIndex, p);
		}

		void GetVertexNormal(Math::Vector3f& p, std::size_t vertexIndex)
		{
			GetVertexProperty("NORMAL", vertexIndex, p);
		}

		void GetVertexTangent(Math::Vector3f& p, std::size_t vertexIndex)
		{
			GetVertexProperty("TANGENT", vertexIndex, p);
		}

		void GetVertexTexCoord(Math::Vector2f& p, std::size_t vertexIndex)
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
		Shape(const Math::Transform& objectToWorld, const Math::Transform& worldToObject) noexcept;
		~Shape() = default;

		virtual bool Intersection(const Math::Ray& r, Math::Scalar* t, HitInfo* hitInfo) const noexcept = 0;
		bool IntersectionFast(const Math::Ray& r) const noexcept
		{
			return Intersection(r, nullptr, nullptr);
		}

		virtual Math::BoundingBox ObjectBound() const noexcept = 0;
		virtual Math::BoundingBox WorldBound() const noexcept;

		virtual std::shared_ptr<TriangleMesh> ConvertToTriangleMesh() const noexcept  = 0;
	
		const Math::Transform& ObjectToWorld;
		const Math::Transform& WorldToObject;
	};
}