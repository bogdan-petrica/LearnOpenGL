#ifndef SD_FW_SCENE_H
#define SD_FW_SCENE_H

#include <vector>
#include <memory>

#include <glm/glm.hpp>

// Fw
class Geometry;
class Object;
class Material;
class Item;

class Scene
{
public:
    Scene();

    void
    init();

    void
    add(Item& item) throw();

    void
    draw();

    std::vector<Item>& items();

private:
    glm::mat4 mWorldTransform;
    std::vector<Item> mItems;
};

//******************************************************************************
inline std::vector<Item>&
Scene::items()
{
    return mItems;
}

#endif