#ifndef SD_FW_WINDOW_H
#define SD_FW_WINDOW_H

#include <vector>
#include <string>
#include <exception>
#include <memory>

#include <glm/glm.hpp>

struct GLFWwindow;

class Window
{
public:
    class IKeyCallback
    {
    public:
        virtual void keyAction(GLFWwindow* window, int key, int scancode, int action, int mode) = 0;
    };

    class WindowCreationException: public std::exception
    {
    public:
        WindowCreationException(const char* failedCall)
        {
            mCacheText.assign("Failed to initialize GLFW window. [");
            mCacheText.append(failedCall);
            mCacheText.append("] call FAILED.");
        }

        const char* what() const throw()
        {
            return mCacheText.c_str();
        }
    private:
        std::string mCacheText;
    };

public:

    static std::shared_ptr<Window> init(int w, int h);
    static void term();

    void
    addKeyHandler(IKeyCallback* handler);

    bool
    shouldClose();
    void
    processEvents();
    void
    swapBuffers();

    GLFWwindow*
    glfwWindow();

private:

    Window();

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    std::vector<IKeyCallback*> mKeyCallbacks;

    static void initGlfw(int w, int h);
    static void termGlfw();
    static bool glfwReady();
    static GLFWwindow* mWindow;

    static std::shared_ptr<Window> mThis;
};

//******************************************************************************
inline GLFWwindow*
Window::glfwWindow()
{
    return mWindow;
}

#endif