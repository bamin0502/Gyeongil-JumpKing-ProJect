#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class Player;
class JumpEffect:public SpriteGo
{
protected:
    struct ClipInfo
    {
        std::string jumpEffect;
        bool filpX=false;
        sf::Vector2f point;
        ClipInfo()
        = default;
        ClipInfo(std::string jumpEffect, bool flipX, const sf::Vector2f& point)
            :jumpEffect(std::move(jumpEffect)), filpX(flipX), point(point) {}
    };
    Animator animator;
    Player* player;
public:
    JumpEffect();
    ~JumpEffect() override;

    void Init() override;
    void Release() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;
    void SetPosition(const sf::Vector2f& position) override;
    void SetScale(const sf::Vector2f& scale) override;
    void PlayAnimation();
};
