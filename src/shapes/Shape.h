#pragma once

#include "../math/MathType.h"
#include "../math/Transform.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace Dash
{
	namespace Math
	{
		struct HitInfo
		{
			Vector3f Position;
			Vector3f Normal;
			Vector2f TexCoord;
			Vector2f LightMapUV;
			Vector3f Tangent;
			Vector3f Bitangent;
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

		struct InputLayoutElement
		{
			std::string SemanticName;
			std::size_t SemanticIndex;
			DASH_FORMAT Format;
			std::size_t AlignedByteOffset;
		};

		struct MeshPart
		{
			std::uint32_t VertexStart;
			std::uint32_t VertexCount;
			std::uint32_t IndexStart;
			std::uint32_t IndexCount;
			std::uint32_t MaterialIdx;

			MeshPart() : VertexStart(0), VertexCount(0), IndexStart(0), IndexCount(0), MaterialIdx(0)
			{
			}
		};

		struct TriangleMesh
		{
			std::vector<std::string> InputElementStrings;
			std::unordered_map<std::string, std::uint16_t> InputElementMap; // pair{ SemanticName, Offset }
			std::vector<InputLayoutElement> InputElements;
			std::vector<MeshPart> MeshParts;
			std::uint32_t VertexStride = 0;
			std::uint32_t NumVertices = 0;
			std::uint32_t NumIndices = 0;
			DASH_FORMAT IndexType;
			std::vector<std::uint8_t> Vertices;
			std::vector<std::uint8_t> Indices;
		}; 

		class Shape
		{
		public:
			Shape(const Transform& objectToWorld, const Transform& worldToObject) noexcept;
			~Shape() = default;

			virtual bool Intersection(const Ray& r, Scalar* t, HitInfo* hitInfo) const noexcept = 0;
			bool IntersectionFast(const Ray& r) const noexcept
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
}