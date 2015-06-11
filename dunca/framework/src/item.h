#if !defined SD_FW_ITEM_H
#define SD_FW_ITEM_H

#include <memory>

class Geometry;
class Object;

class Item
{
public:
    Item(std::shared_ptr<Geometry> geometry,
        std::shared_ptr<Object> object = nullptr);

    std::shared_ptr<Geometry> getGeometry();
    std::shared_ptr<Object> getObject();

private:

    std::shared_ptr<Geometry> mGeometry;
    std::shared_ptr<Object> mObject;
};

inline std::shared_ptr<Geometry>
Item::getGeometry()
{
    return mGeometry;
}

inline std::shared_ptr<Object>
Item::getObject()
{
    return mObject;
}

#endif // #ifdef SD_FW_OBJECT_H