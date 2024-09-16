#pragma once
#include "Actor.h"
class ClearPict : public Actor
{
public:
    ClearPict(Game* game);
    void UpdateActor(float deltaTime) override;

private:
    class SpriteComponent* sc;
};