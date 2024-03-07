#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class Player:public SpriteGo
{
    enum playerState
    {
        idle,
        move,
        jump,
        jumpup,
        jumpdown
    };
    struct ClipInfo
    {
        std::string idle;
        std::string move;
        std::string jump;
        std::string jumpup;
        std::string jumpdown;
        bool filpX=false;
        sf::Vector2f point;
        ClipInfo()
        {
            
        }
        ClipInfo(const std::string& idle, const std::string& move,const std::string& jump,const std::string& jumpup,const std::string& jumpdown, bool flipX, const sf::Vector2f& point)
            :idle(idle), move(move), jump(jump),jumpup(jumpup),jumpdown(jumpdown),filpX(flipX), point(point) {}
        
    };
protected:
    Animator animator;

    float moveSpeed=300.f;
    sf::Vector2f velocity;
    bool isGrounded=true;
    bool isFalling=false;
    bool isJumpCharging=false;
    float gravity=100.f;

    float jumpHeight;
    float jumpDirection;
    float jumpStartTime=0;
    float jumpTime=0.f;
    float jumpPower=300.f;
    float jumpGuage;
    float maxjumpTime=0.6f;
    float jumpStep=35.f;
    bool isJumping=false;
    int currentJumpStage;
    float currentHeight;
    sf::Clock timer;
    
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

    void CheckCollision();
    
};
