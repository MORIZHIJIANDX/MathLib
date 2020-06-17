#include "Shape.h"

namespace Dash
{
	Shape::Shape(const Transform& objectToWorld, const Transform& worldToObject) noexcept
		: ObjectToWorld(objectToWorld)
		, WorldToObject(worldToObject)
	{
	}

	BoundingBox Shape::WorldBound() const noexcept
	{
		return ObjectToWorld.TransformBoundingBox(ObjectBound());
	}
}


