#include "window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cassert>

std::shared_ptr<Window> Window::mThis;
GLFWwindow* Window::mWindow = nullptr;

//******************************************************************************
Window::Window()
{
}

//******************************************************************************
std::shared_ptr<Window>
Window::init(int w, int h)
{
    mThis.reset(new Window());
    if(!glfwReady())
    {
        initGlfw(w, h);
    }
    return mThis;
}

//******************************************************************************
void
Window::term()
{
    termGlfw();
}

//******************************************************************************
void
Window::addKeyHandler(IKeyCallback* handler)
{
    mKeyCallbacks.push_back(handler);
}

//******************************************************************************
void
Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    for (int i = 0; i < mThis->mKeyCallbacks.size(); ++i)
    {
        assert(mThis->mKeyCallbacks[i] != nullptr);
        mThis->mKeyCallbacks[i]->keyAction(window, key, scancode, action, mode);
    }
}

//******************************************************************************
void
Window::initGlfw(int w, int h)
{
    if(!glfwReady())
    {
        if(glfwInit() != GL_TRUE)
            throw new WindowCreationException("glfwInit");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        mWindow = glfwCreateWindow(w, h, "Hello Coordinate Systems and Camera (dunca)", NULL, NULL);
        if(mWindow == nullptr)
            throw new WindowCreationException("glfwCreateWindow");

        glfwMakeContextCurrent(mWindow);

        glewExperimental = GL_TRUE;
        if(glewInit() != GLEW_OK)
            throw new WindowCreationException("glewInit");

        glfwSetKeyCallback( mWindow, &key_callback );
    }
}

//******************************************************************************
void
Window::termGlfw()
{
    if(mWindow != nullptr)
    {
        glfwSetWindowShouldClose(mWindow, GL_TRUE);
    }
    glfwTerminate();
}

//******************************************************************************
bool
Window::glfwReady()
{
    return mWindow != nullptr;
}

//******************************************************************************
bool
Window::shouldClose()
{
    return glfwWindowShouldClose(mWindow);
}

//******************************************************************************
void
Window::processEvents()
{
    glfwPollEvents();
}

//******************************************************************************
void
Window::swapBuffers()
{
    glfwSwapBuffers(mWindow);
}