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

const GLfloat bkColor[3] = { 0.2f, 0.3f, 0.3f };

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
}

//******************************************************************************
void
Renderer::runLoop() throw()
{
    while(!mWindow->shouldClose())
    {
        mWindow->processEvents();

        // Draw background to cover the previous frame
        glClearColor( bkColor[0], bkColor[1], bkColor[2], 1.0f );
        glClear(GL_COLOR_BUFFER_BIT);

        draw();

        mWindow->swapBuffers();
    }
}

void
Renderer::draw()
{
    glm::mat4 viewMat;
    if(mCamera != nullptr)
    {
        const glm::vec4& v = mCamera->viewport();
        glViewport(v[0], v[1], v[2], v[3]);

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
            std::shared_ptr<Material> material = geometry.getMaterial();
            if(material != nullptr)
            {
                material->bind();
            }
            std::shared_ptr<Object> obj = item.getObject();
            if(obj != nullptr)
            {
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
        }
    }
}
