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
    // 차징 시간이 1초를 초과했는지 확인하고, 조건을 충족하면 자동으로 점프 실행
    if (isJumpCharging) {
        float chargingTime = timer.getElapsedTime().asSeconds() - jumpStartTime;
        if (chargingTime > 1.0f) { 
            PerformJump(); // 자동 점프 실행
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
    jumpDirection = 0.f;
    if (InputMgr::GetKey(sf::Keyboard::Left)) {
        jumpDirection = -1.f;
        sprite.setScale(-1.f, 1.f);
    } else if (InputMgr::GetKey(sf::Keyboard::Right)) {
        jumpDirection = 1.f;
        sprite.setScale(1.f, 1.f);
    }

    if (InputMgr::GetKeyDown(sf::Keyboard::Space) && isGrounded) {
        StartJumpCharging();
    }
    // 여기에 isJumpCharging 조건을 명확히 추가합니다.
    if (InputMgr::GetKeyUp(sf::Keyboard::Space) && isJumpCharging) {
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
            animator.Play("animations/player_Jump.csv");
            break;
        case JumpPhase::Rising:
            animator.Play("animations/player_JumpUp.csv");
            break;
        case JumpPhase::Falling:
            animator.Play("animations/player_JumpDown.csv");
            break;
        case JumpPhase::Grounded:
            if (std::abs(velocity.x) < 0.1f) {
                animator.Play("animations/player_Idle.csv");
            } else {
                animator.Play("animations/player_Move.csv");
            }
            break;
    }
}

void Player::StartJumpCharging() {
    isJumpCharging = true;
    jumpPhase = JumpPhase::Charging;
    jumpStartTime = timer.getElapsedTime().asSeconds();
}

void Player::PerformJump() {
    // 차징 상태 해제 및 점프 상태 설정
    isJumpCharging = false;
    isJumping = true;
    jumpPhase = JumpPhase::Rising;

    float chargeDuration = timer.getElapsedTime().asSeconds() - jumpStartTime;
    float jumpHeight = sqrt(8 * gravity * jumpHeightFactor * chargeDuration); // 계산된 점프 높이

    // 최대 점프 높이를 초과하지 않도록 조정
    if (jumpHeight > maxJumpHeight) {
        jumpHeight = maxJumpHeight;
    }

    velocity.y = -jumpHeight; // Y축 속도에 점프 높이 적용
    velocity.x = jumpDirection * moveSpeed * 0.5;

    // 스프라이트 반전 설정
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
                return true;
            }
        }
    }
    return false;
}
