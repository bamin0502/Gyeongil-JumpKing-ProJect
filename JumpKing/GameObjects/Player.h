#pragma once
#include "SpriteGo.h"
#include "Animator.h"


class GameScene;

class Player:public SpriteGo
{
public:
    enum class CollisionType
    {
        NONE=0,
        LEFT=1<<0,
        RIGHT=1<<1,
        TOP=1<<2,
        BOTTOM=1<<3,
    };
    CollisionType collisionType;
    
    enum class PlayerPhase {
        Grounded,
        Charging,
        Rising,
        Falling,
        Landing
    };
    PlayerPhase playerPhase;
private:
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
        = default;

        ClipInfo(const std::string& idle, const std::string& move,const std::string& jump,const std::string& jumpup,const std::string& jumpdown, bool flipX, const sf::Vector2f& point)
            :idle(idle), move(move), jump(jump),jumpup(jumpup),jumpdown(jumpdown),filpX(flipX), point(point) {}
    };
    
protected:
    Animator animator;
    sf::Vector2f velocity;
    sf::Clock timer;
    GameScene* gameScene=nullptr;
    sf::Image collisionMap;
    sf::FloatRect playerBounds;
    sf::FloatRect mapBounds;

    //플레이어 이동을 판정하게할 충돌체크형 Bool문
    bool isCollidingLeft=false;
    bool isCollidingRight=false;
    bool isCollidingTop=false;
    bool isCollidingBottom=false;
    
    //디버그 및 테스트
    sf::RectangleShape testRectangle;
    bool debugMode=false;
    bool testMode=false;
    
    float moveSpeed = 100.f;
    bool isGrounded=true;
    bool isFalling=false;
    bool isJumpCharging=false;
    float gravity = 100.f;
    float jumpHeightFactor = 100.f;
    float jumpHeight;
    float jumpDirection;
    float jumpStartTime = 0;
    float jumpTime = 0.f;
    float jumpPower = 1000.f;
    float jumpGuage;
    float maxjumpTime = 0.6f;
    float jumpStep = 35.f;
    bool isJumping=false;
    int currentJumpStage;
    float currentHeight;
    float maxJumpHeight = 500.0f;

public:
    Player(const std::string& name = "");

    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;
    void SetActive(bool active) override;

    void HandleInput(float dt);
    void UpdateMovement(float dt);
    void UpdateAnimation();
    void StartJumpCharging();
    void PerformJump();
    
    CollisionType CheckCollision();
};
