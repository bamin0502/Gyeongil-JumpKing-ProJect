#include "pch.h"
#include "Player.h"
#include "GameScene.h"

Player::Player(const std::string& name)
    : SpriteGo(name), jumpPhase(JumpPhase::Grounded), gameScene(nullptr), jumpHeight(0), jumpDirection(0.f),
      jumpGuage(0),
      currentJumpStage(0),
      currentHeight(0)
{
}

void Player::Init() {
    animator.SetTarget(&sprite);
    SetPosition({0.f, 210.f});
}

void Player::Release() {
}

void Player::Reset() {
    gameScene = dynamic_cast<GameScene*>(SCENE_MGR.GetCurrentScene());
    collisionMap=gameScene->map1Sprite.getTexture()->copyToImage();
    animator.Play("animations/player_Idle.csv");
    SetOrigin(Origins::BC);
    
    jumpPhase = JumpPhase::Grounded;
}

void Player::Update(float dt) {
    SpriteGo::Update(dt);
    animator.Update(dt);

    if(position.y>groundYPosition)
    {
        position.y=groundYPosition;
    }
    HandleInput();
    CheckCollision();
    
    if (isJumpCharging) {
        const float chargingTime = timer.getElapsedTime().asSeconds() - jumpStartTime;
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

void Player::UpdateMovement(const float dt) {
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
    default: ;
    }
}

void Player::StartJumpCharging() {
    isJumpCharging = true;
    jumpPhase = JumpPhase::Charging;
    jumpStartTime = timer.getElapsedTime().asSeconds();
}

float calculateJumpHeight(float chargeDuration, float maxJumpHeight)
{
    constexpr int levels = 35;
    constexpr float maxChargeTime = 1.5f;
    constexpr float levelDuration = maxChargeTime / levels;
    const int level = std::min(static_cast<int>(chargeDuration / levelDuration), levels);
    const float jumpHeight = maxJumpHeight * (level / static_cast<float>(levels));

    return jumpHeight;
}
void Player::PerformJump() {
    const float chargeDuration = timer.getElapsedTime().asSeconds() - jumpStartTime;
    const float jumpHeight = calculateJumpHeight(chargeDuration, maxJumpHeight);

    isJumpCharging = false;
    isJumping = true;
    jumpPhase = JumpPhase::Rising;
    
    velocity.y = -sqrt(2 * gravity * jumpHeight); 
    velocity.x = jumpDirection * moveSpeed * 0.5; 

    sprite.setScale(jumpDirection < 0 ? -1.f : 1.f, 1.f);

}

void Player::CheckCollision()
{
    sf::FloatRect playerViewBounds = sprite.getGlobalBounds();
    sf::Vector2u mapSize = collisionMap.getSize();
    
    for (int x = static_cast<int>(playerViewBounds.left); x < static_cast<int>(playerViewBounds.left + playerViewBounds.width); x++) {
        for (int y = static_cast<int>(playerViewBounds.top); y < static_cast<int>(playerViewBounds.top + playerViewBounds.height); y++) {
            if (x >= 0 && x < static_cast<int>(mapSize.x) && y >= 0 && y < static_cast<int>(mapSize.y)) {
                if (collisionMap.getPixel(x, y).a != 0) {
                    HandleCollisionResponse(sf::FloatRect(static_cast<float>(x), static_cast<float>(y), 1.f, 1.f));
                    
                }
            }
        }
    }
    
    
    // const sf::Vector2f playerPixelPos = gameScene->PlayerBoundsWorldToView(position);
    // const sf::IntRect playerBounds(
    //     static_cast<int>(playerPixelPos.x),
    //     static_cast<int>(playerPixelPos.y),
    //     static_cast<int>(sprite.getGlobalBounds().width),
    //     static_cast<int>(sprite.getGlobalBounds().height)
    // );
    //
    // const sf::Vector2u mapSize = collisionMap.getSize();
    //
    // for (int x = playerBounds.left; x < playerBounds.left + playerBounds.width; x++) {
    //     for (int y = playerBounds.top; y < playerBounds.top + playerBounds.height; y++) {
    //       
    //         if (x >= 0 && x < static_cast<int>(mapSize.x) && y >= 0 && y < static_cast<int>(mapSize.y)) {
    //             if (collisionMap.getPixel(x, y).a != 0) {
    //                 
    //                 HandleCollisionResponse(sf::FloatRect(
    //                     static_cast<float>(x), 
    //                     static_cast<float>(y), 
    //                     1.0f, 
    //                     1.0f
    //                 ));
    //             }
    //         }
    //     }
    // }
}

void Player::HandleCollisionResponse(const sf::FloatRect& globalBounds)
{
    if (velocity.y > 0) {
        position.y = globalBounds.top - playerBounds.height; // 플레이어를 충돌 지점 바로 위로 이동
        velocity.y = 0; 
        isJumping = false; 
        jumpPhase = JumpPhase::Grounded;
    }
    else if (velocity.y < 0) { 
        position.y = globalBounds.top + globalBounds.height; // 플레이어를 충돌 지점 바로 아래로 이동
        velocity.y = 0; 
    }

    if (velocity.x > 0) { 
        position.x = globalBounds.left - playerBounds.width; // 플레이어를 충돌 지점 바로 왼쪽으로 이동
    }
    else if (velocity.x < 0) { 
        position.x = globalBounds.left + globalBounds.width; // 플레이어를 충돌 지점 바로 오른쪽으로 이동
    }
}





