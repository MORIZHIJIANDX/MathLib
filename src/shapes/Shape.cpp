#include "Shape.h"

namespace Dash
{
	Shape::Shape(const Math::Transform& objectToWorld, const Math::Transform& worldToObject) noexcept
		: ObjectToWorld(objectToWorld)
		, WorldToObject(worldToObject)
	{
	}

	Math::BoundingBox Shape::WorldBound() const noexcept
	{
		return ObjectToWorld.TransformBoundingBox(ObjectBound());
	}
}


