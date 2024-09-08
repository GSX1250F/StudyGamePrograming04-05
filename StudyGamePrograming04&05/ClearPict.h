#pragma once
#include "Actor.h"
class ClearPict : public Actor
{
public:
    ClearPict(Game* game);

    class SpriteComponent* GetSprite() { return sc; }

private:
    class SpriteComponent* sc;
};

