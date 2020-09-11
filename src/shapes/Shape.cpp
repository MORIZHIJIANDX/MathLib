#include "Shape.h"

namespace Dash
{
	Shape::Shape(const FTransform& objectToWorld, const FTransform& worldToObject) noexcept
		: ObjectToWorld(objectToWorld)
		, WorldToObject(worldToObject)
	{
	}

	FBoundingBox Shape::WorldBound() const noexcept
	{
		return ObjectToWorld.TransformBoundingBox(ObjectBound());
	}
}


