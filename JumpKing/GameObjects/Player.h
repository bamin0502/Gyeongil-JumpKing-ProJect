#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class GameScene;

class Player:public SpriteGo
{
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
public:
    enum class JumpPhase {
        Grounded,
        Charging,
        Rising,
        Falling,
        Landing
    };
    JumpPhase jumpPhase;
protected:
    Animator animator;
    sf::Vector2f velocity;
    sf::Clock timer;
    GameScene* gameScene;
    sf::Image collisionMap;
    sf::FloatRect playerBounds;
    
    float moveSpeed=300.f;
    bool isGrounded=true;
    bool isFalling=false;
    bool isJumpCharging=false;
    float gravity=300.f;
    float jumpHeightFactor=100.f;
    float jumpHeight;
    float jumpDirection;
    float jumpStartTime=0;
    float jumpTime=0.f;
    float jumpPower=1000.f;
    float jumpGuage;
    float maxjumpTime=0.6f;
    float jumpStep=35.f;
    bool isJumping=false;
    int currentJumpStage;
    float currentHeight;
    float maxJumpHeight = 380.0f;
    float groundYPosition=210.f;

public:
    Player(const std::string& name = "");

    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;

    void HandleInput();
    void UpdateMovement(float dt);
    void UpdateAnimation();
    void StartJumpCharging();
    void PerformJump();
    void CheckCollision();
    
    void HandleCollisionResponse(const sf::FloatRect& globalBounds);
};
