#include "pch.h"
#include "Player.h"
#include "GameScene.h"

Player::Player(const std::string& name)
    : SpriteGo(name), jumpDirection(0.f), jumpPhase(JumpPhase::Grounded) {
}

void Player::Init() {
    animator.SetTarget(&sprite);
}

void Player::Release() {
}

void Player::Reset() {
    animator.Play("animations/player_Idle.csv");
    SetOrigin(Origins::BC);
    SetPosition({0.f, 510.f});
    jumpPhase = JumpPhase::Grounded;
}

void Player::Update(float dt) {
    SpriteGo::Update(dt);
    animator.Update(dt);
    
    HandleInput();
    if (isJumpCharging) {
        float chargingTime = timer.getElapsedTime().asSeconds() - jumpStartTime;
        if (chargingTime > 1.0f) { 
            PerformJump(); 
        }
    }
    UpdateMovement(dt);
    UpdateAnimation();
    
    SetPosition(position);
}

void Player::LateUpdate(float dt) {
    SpriteGo::LateUpdate(dt);
}

void Player::FixedUpdate(float dt) {
    SpriteGo::FixedUpdate(dt);
}

void Player::Draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Player::HandleInput() {
    if (!isJumping) {
        jumpDirection = 0.f;
        if (InputMgr::GetKey(sf::Keyboard::Left)) {
            jumpDirection = -1.f;
            sprite.setScale(-1.f, 1.f); 
        } else if (InputMgr::GetKey(sf::Keyboard::Right)) {
            jumpDirection = 1.f;
            sprite.setScale(1.f, 1.f); 
        }
    }

    
    if (InputMgr::GetKeyDown(sf::Keyboard::Space) && isGrounded && !isJumping) {
        StartJumpCharging();
    }

    
    if (InputMgr::GetKeyUp(sf::Keyboard::Space) && isJumpCharging && !isJumping) {
        PerformJump();
    }
}

void Player::UpdateMovement(float dt) {
    switch (jumpPhase) {
        case JumpPhase::Rising:
        case JumpPhase::Falling:
            velocity.y += gravity * dt;
            position += velocity * dt;
            if (position.y > groundYPosition) {
                position.y = groundYPosition;
                velocity.y = 0;
                jumpPhase = JumpPhase::Grounded;
                isJumping = false;
                isGrounded = true;
            }
            break;
        case JumpPhase::Grounded:
            velocity.x = jumpDirection * moveSpeed;
            position += velocity * dt;
            break;
        default:
            break;
    }
}

void Player::UpdateAnimation() {
    switch (jumpPhase) {
    case JumpPhase::Charging:
        if (animator.GetCurrentClipId() != "animations/player_Jump.csv") {
            animator.Play("animations/player_Jump.csv");
        }
        break;
    case JumpPhase::Rising:
        if (animator.GetCurrentClipId() != "animations/player_JumpUp.csv") {
            animator.Play("animations/player_JumpUp.csv");
        }
        break;
    case JumpPhase::Falling:
        if (animator.GetCurrentClipId() != "animations/player_JumpDown.csv") {
            animator.Play("animations/player_JumpDown.csv");
        }
        break;
    case JumpPhase::Grounded:
        // 점프 후 이동 중인 경우 Move 애니메이션으로 전환
            if (std::abs(velocity.x) >= 0.1f) {
                if (animator.GetCurrentClipId() != "animations/player_Move.csv") {
                    animator.Play("animations/player_Move.csv");
                }
            } else {
                if (animator.GetCurrentClipId() != "animations/player_Idle.csv") {
                    animator.Play("animations/player_Idle.csv");
                }
            }
        break;
    }
}

void Player::StartJumpCharging() {
    isJumpCharging = true;
    jumpPhase = JumpPhase::Charging;
    jumpStartTime = timer.getElapsedTime().asSeconds();
}

float calculateJumpHeight(float chargeDuration, float maxJumpHeight)
{
    const int levels = 35; 
    const float maxChargeTime = 1.5f; 
    float levelDuration = maxChargeTime / levels; 
    int level = std::min(static_cast<int>(chargeDuration / levelDuration), levels);
    float jumpHeight = maxJumpHeight * (level / static_cast<float>(levels));

    return jumpHeight;
}
void Player::PerformJump() {
    float chargeDuration = timer.getElapsedTime().asSeconds() - jumpStartTime;
    float jumpHeight = calculateJumpHeight(chargeDuration, maxJumpHeight);

    isJumpCharging = false;
    isJumping = true;
    jumpPhase = JumpPhase::Rising;
    
    velocity.y = -sqrt(2 * gravity * jumpHeight); 
    velocity.x = jumpDirection * moveSpeed * 0.5; 

    sprite.setScale(jumpDirection < 0 ? -1.f : 1.f, 1.f); 
}

bool Player::CheckCollision(const sf::Image& image) {
    sf::Vector2i playerPosition=static_cast<sf::Vector2i>(this->GetPosition());

    sf::Color obstacleColor=sf::Color::Red;

    for(int y=0; y<sprite.getGlobalBounds().height; ++y)
    {
        for(int x=0; x<sprite.getGlobalBounds().width; ++x)
        {
            sf::Vector2i pixelPosition=playerPosition+sf::Vector2i(x,y);
            if(pixelPosition.x<0 || pixelPosition.x>=image.getSize().x || pixelPosition.y<0 || pixelPosition.y>=image.getSize().y)
            {
                continue;
            }
            if(image.getPixel(pixelPosition.x,pixelPosition.y)==obstacleColor)
            {
                std::cout<<"Collision detected"<<std::endl;
                return true;
                
            }
        }
    }
    return false;
}
