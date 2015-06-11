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

    virtual void
    prepare();
    virtual void
    prepareFrame();
    virtual void
    beginItemDraw(Item& item);
    virtual void
    endItemDraw(Item& item);

    void
    add(Item& item) throw();

    std::vector<Item>& items();

private:
    std::vector<Item> mItems;
    const glm::vec3 mBkColor;
};

//******************************************************************************
inline std::vector<Item>&
Scene::items()
{
    return mItems;
}

#endif