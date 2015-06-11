#include "scene.h"

#include "item.h"
#include "object.h"
#include "material.h"
#include "geometry.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Scene::Scene()
    : mBkColor(0.0f, 0.0f, 0.0f)
{
}

//******************************************************************************
void
Scene::init()
{
}

//******************************************************************************
void
Scene::prepare()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
}

void
Scene::prepareFrame()
{
    // Draw background to cover the previous frame
    glClearColor(mBkColor.r, mBkColor.g, mBkColor.b, 1.0f);
    // Clear depth buffer as well
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
Scene::beginItemDraw(Item& item)
{
    std::shared_ptr<Geometry> geometry = item.getGeometry();
    if(geometry != nullptr)
    {
        if(!geometry->isBackfaceCooled())
        {
            // Disable backface cooling
            glDisable(GL_CULL_FACE);
        }
    }
}

void
Scene::endItemDraw(Item& item)
{
    std::shared_ptr<Geometry> geometry = item.getGeometry();
    if(geometry != nullptr)
    {
        if(!geometry->isBackfaceCooled())
        {
            // Disable backface cooling
            glEnable(GL_CULL_FACE);
        }
    }
}

//******************************************************************************
void
Scene::add(Item& item) throw()
{
    mItems.push_back(item);
}