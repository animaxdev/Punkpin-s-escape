#pragma once

#include "CWorldObject.h"
#include "defines.h"


class CItem : public CWorldObject
{
private:

    bool consumable;
    int value;
    weaponIdData data;


public:

    CItem(position pos);
    CItem(weaponIdData _data, position pos);
    ~CItem() {}

    bool update() { }    // todo

    weaponIdData getData() const { return data; }
    bool isConsumable() const { return consumable; }
    int getValue() const { return value; }
    std::string getName() const { return data.name; }
};
