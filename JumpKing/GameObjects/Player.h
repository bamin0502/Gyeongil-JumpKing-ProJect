#pragma once
#include "SpriteGo.h"

class Player:public SpriteGo
{
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
    Player();
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
