#include "pch.h"
#include "SpriteGoEffect.h"

SpriteGoEffect::SpriteGoEffect(const std::string& name):SpriteGo(name)
{
}

SpriteGoEffect::~SpriteGoEffect()
{
}

void SpriteGoEffect::Update(float dt)
{
    timer += dt;
    if(timer>duration)
    {
        SetActive(false);
        SCENE_MGR.GetCurrentScene()->RemoveGo(this);
    }
}

void SpriteGoEffect::Reset()
{
    SpriteGo::Reset();
    timer = 0.f;
    duration = 3.f;
}
