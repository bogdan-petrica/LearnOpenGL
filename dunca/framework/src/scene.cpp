#include "scene.h"

#include "item.h"
#include "object.h"
#include "material.h"
#include "geometry.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Scene::Scene()
{
}

//******************************************************************************
void
Scene::init()
{
    
}

//******************************************************************************
void
Scene::add(Item& item) throw()
{
    mItems.push_back(item);
}