﻿#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class Player:public SpriteGo
{
    struct ClipInfo
    {
        std::string idle;
        std::string move;
        bool filpX=false;
        sf::Vector2f point;
        ClipInfo()
        {
            
        }
        ClipInfo(const std::string& idle, const std::string& move, bool flipX, const sf::Vector2f& point)
            :idle(idle), move(move), filpX(flipX), point(point) {}
    };
protected:
    Animator animator;

    float moveSpeed=100.f;
    sf::Vector2f velocity;
    bool isGrounded=true;

    float gravity=500.f;
    
    float jumpTime=0.6f;
    float jumpPower=300.f;
    int jumpStep=35;
    bool isJumping=false;
    
public:
    Player(const std::string& name = "");
    ~Player() override;

    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;

    void Jump();
    void Move(float dt);
    void CheckCollision();
    
};
