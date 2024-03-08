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
    SetPosition({0.f, 500.f});
    jumpPhase = JumpPhase::Grounded;
}

void Player::Update(float dt) {
    SpriteGo::Update(dt);
    animator.Update(dt);
    HandleInput();
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
    // 방향키 입력 처리
    float directionInput = 0.f;
    if (InputMgr::GetKey(sf::Keyboard::Left)) {
        directionInput -= 1.f;
        sprite.setScale(-1.f, 1.f); // 왼쪽으로 이동 시 스프라이트 반전
    }
    if (InputMgr::GetKey(sf::Keyboard::Right)) {
        directionInput += 1.f;
        sprite.setScale(1.f, 1.f); // 오른쪽으로 이동 시 스프라이트 정방향
    }

    if (jumpPhase == JumpPhase::Grounded) {
        jumpDirection = directionInput;

        if (InputMgr::GetKeyDown(sf::Keyboard::Space)) {
            StartJumpCharging();
        }
    } else if (jumpPhase == JumpPhase::Charging && InputMgr::GetKeyUp(sf::Keyboard::Space)) {
        // 차징 중 방향키를 고려하여 점프 방향 결정
        jumpDirection = directionInput;
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
    jumpPhase = JumpPhase::Charging;
    jumpStartTime = timer.getElapsedTime().asSeconds();
}

void Player::PerformJump() {
    isJumpCharging = false;
    isJumping = true;
    float chargeTime = std::min(timer.getElapsedTime().asSeconds() - jumpStartTime, maxjumpTime);
    velocity.y = -sqrt(8 * gravity * (100.f * chargeTime));

    if (jumpDirection != 0) {
        velocity.x = jumpDirection * (moveSpeed * 0.5);
    }
    else {
        velocity.x = 0;
    }

    jumpPhase = JumpPhase::Rising;
}

void Player::CheckCollision() {
}
