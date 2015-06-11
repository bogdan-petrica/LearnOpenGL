#define GLEW_STATIC
#include "item.h"

#include "geometry.h"
#include "object.h"

//******************************************************************************
Item::Item(std::shared_ptr<Geometry> geometry,
      std::shared_ptr<Object> object/*=nullptr*/)
    : mGeometry(geometry)
    , mObject(object)
{
}
