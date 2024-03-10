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
    gameScene = dynamic_cast<GameScene*>(SCENE_MGR.GetCurrentScene());
    collisionMap=gameScene->map1Texture;
    animator.Play("animations/player_Idle.csv");
    SetOrigin(Origins::BC);
    
    jumpPhase = JumpPhase::Grounded;
}

void Player::Update(float dt) {
    SpriteGo::Update(dt);
    animator.Update(dt);

    HandleInput();
    if(gameScene->IsPlayerInView(position))
    {
        CheckCollision();
    }
    else
    {
        isGrounded = false;
    }
    
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

void Player::CheckCollision()
{
    sf::Vector2f playerPixelPos = gameScene->PlayerBoundsWorldToView(position);
    sf::IntRect playerBounds(playerPixelPos.x, playerPixelPos.y, sprite.getLocalBounds().width, sprite.getLocalBounds().height);

    for (int x = playerBounds.left; x < playerBounds.left + playerBounds.width; x++) {
        for (int y = playerBounds.top; y < playerBounds.top + playerBounds.height; y++) {
            if (collisionMap.getPixel(x, y).a == 255) {
                HandleCollisionResponse(GetGlobalBounds());
                return;
            }
        }
    }
}

void Player::HandleCollisionResponse(const sf::FloatRect& globalBounds)
{
    if (velocity.y > 0) { // 하강 중이라면
        position.y = globalBounds.top - playerBounds.height; // 플레이어를 충돌 지점 바로 위로 이동
        velocity.y = 0; // Y축 속도를 0으로 설정하여 추가 하강을 방지
        isJumping = false; // 점프 상태 해제
        jumpPhase = JumpPhase::Grounded; // 점프 상태를 '지면에 있음'으로 변경
    }
    else if (velocity.y < 0) { // 상승 중이라면
        position.y = globalBounds.top + globalBounds.height; // 플레이어를 충돌 지점 바로 아래로 이동
        velocity.y = 0; // Y축 속도를 0으로 설정하여 추가 상승을 방지
    }

    if (velocity.x > 0) { // 우측 이동 중이라면
        position.x = globalBounds.left - playerBounds.width; // 플레이어를 충돌 지점 바로 왼쪽으로 이동
    }
    else if (velocity.x < 0) { // 좌측 이동 중이라면
        position.x = globalBounds.left + globalBounds.width; // 플레이어를 충돌 지점 바로 오른쪽으로 이동
    }
}





