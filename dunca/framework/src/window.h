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
    class IInputCallback
    {
    public:
        virtual void keyAction(GLFWwindow* window, int key, int scancode, int action, int mode) = 0;
        virtual void mouseAction(GLFWwindow* window, double xpos, double ypos) = 0;
        virtual void scrollAction(GLFWwindow* window, double xoffset, double yoffset) = 0;
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
    ~Window();

    static std::shared_ptr<Window> init(int w, int h);
    static void term();

    void
    addKeyHandler(IInputCallback* handler);

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
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    std::vector<IInputCallback*> mKeyCallbacks;

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