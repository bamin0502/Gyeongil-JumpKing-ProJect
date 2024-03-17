#include "pch.h"
#include "JumpEffect.h"
#include "Player.h"

JumpEffect::JumpEffect(): SpriteGo(name), player(nullptr)
{
}

JumpEffect::~JumpEffect()
= default;

void JumpEffect::Init()
{
    SpriteGo::Init();
    
}

void JumpEffect::Release()
{
    SpriteGo::Release();
}

void JumpEffect::Update(float dt)
{
    SpriteGo::Update(dt);
    PlayAnimation();
    
}

void JumpEffect::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);
    
}



void JumpEffect::SetPosition(const sf::Vector2f& position)
{
    SpriteGo::SetPosition(position);
}

void JumpEffect::SetScale(const sf::Vector2f& scale)
{
    SpriteGo::SetScale(scale);
}

void JumpEffect::PlayAnimation()
{
    if(animator.GetCurrentClipId() != "animations/player_JumpEffect.csv")
    {
        animator.Play("animations/player_JumpEffect.csv");
    }
    
}
