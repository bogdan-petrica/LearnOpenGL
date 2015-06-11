#include "renderer.h"

#include "window.h"
#include "scene.h"
#include "material.h"
#include "camera.h"
#include "item.h"
#include "object.h"
#include "geometry.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//******************************************************************************
Renderer::Renderer()
{
}

//******************************************************************************
void
Renderer::init(std::shared_ptr<Window> window, std::shared_ptr<Scene> scene,
    std::shared_ptr<Camera> camera)
{
    mWindow = window;
    mScene = scene;
    mCamera = camera;
    assert(mWindow != nullptr);
    assert(mScene != nullptr);
    assert(mCamera != nullptr);
}

//******************************************************************************
void
Renderer::runLoop() throw()
{
    for(int i = 0; i < mRenderEventsListener.size(); ++i)
    {
        mRenderEventsListener[i]->setupScene();
    }

    mScene->prepare();

    while(!mWindow->shouldClose())
    {
        mWindow->processEvents();

        for(int i = 0; i < mRenderEventsListener.size(); ++i)
        {
            mRenderEventsListener[i]->beginFrame();
        }

        draw();

        mWindow->swapBuffers();

        for(int i = 0; i < mRenderEventsListener.size(); ++i)
        {
            mRenderEventsListener[i]->endFrame();
        }
    }
}

//******************************************************************************
void
Renderer::addRenderEventListerner(IRenderEvents* listener)
{
    mRenderEventsListener.push_back(listener);
}

//******************************************************************************
void
Renderer::draw()
{
    mScene->prepareFrame();

    glm::mat4 viewMat;
    {
        const glm::vec4& v = mCamera->viewport();
        glViewport(v[0], v[1], v[2], v[3]);

        // TODO: Take position from camera
        viewMat = mCamera->projection();
        viewMat = glm::translate(viewMat, glm::vec3(0.0f, 0.0f, -3.0f));
    }

    std::vector<Item>& items = mScene->items();
    for(int i = 0; i < items.size(); ++i)
    {
        Item& item = items[i];
        if(item.getGeometry() != nullptr)
        {
            Geometry& geometry = *(item.getGeometry());

            mScene->beginItemDraw(item);

            // First prepare the geometry ...
            geometry.prepare();

            std::shared_ptr<Material> material = geometry.getMaterial();
            if(material != nullptr)
            {
                // ... then the material ...
                material->prepare();
                material->bind();
            }
            std::shared_ptr<Object> obj = item.getObject();
            if(obj != nullptr)
            {
                // ... and later the object.
                obj->prepare();
                if(material != nullptr)
                {
                    glm::mat4 tmpMat;
                    obj->computeModelMatrix(tmpMat);
                    if(material->hasModelUniformLoc())
                    {
                        glUniformMatrix4fv(material->modelUniformLoc(), 1, GL_FALSE, glm::value_ptr(tmpMat));
                    }

                    if(material->hasViewUniformLoc())
                    {
                        glUniformMatrix4fv(material->viewUniformLoc(), 1, GL_FALSE, glm::value_ptr(viewMat));
                    }
                }
            }
            geometry.draw();

            //  prepare the geometry ...
            geometry.close();

            mScene->endItemDraw(item);
        }
    }
}
