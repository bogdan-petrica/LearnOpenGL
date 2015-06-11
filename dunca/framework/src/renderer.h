#ifndef SD_FW_RENDERER_H
#define SD_FW_RENDERER_H

#include <memory>

#include <glm/glm.hpp>

// Fw
class Window;
class Scene;
class Camera;

class Renderer
{
public:
    Renderer();

    void
    init(std::shared_ptr<Window> window, std::shared_ptr<Scene> scene,
        std::shared_ptr<Camera> camera);

    void
    runLoop() throw();

private:
    void
    draw();

    std::shared_ptr<Window> mWindow;
    std::shared_ptr<Scene> mScene;
    std::shared_ptr<Camera> mCamera;
};

#endif