#include "pch.h"
#include "Player.h"

#include "GameScene.h"

Player::Player(const std::string& name)
    : SpriteGo(name),jumpDirection(0.f)
{
}



void Player::Init()
{
    animator.SetTarget(&sprite);
}

void Player::Release()
{
    
}

void Player::Reset()
{
    animator.Play("animations/player_Idle.csv");
    SetOrigin(Origins::BC);
    SetPosition({0.f, 500.f});
}

void Player::Update(float dt)
{
    SpriteGo::Update(dt);

    animator.Update(dt);

    HandleInput();
    UpdateMovement(dt);
    UpdateAnimation();

    SetPosition(position);
}

void Player::LateUpdate(float dt)
{
    SpriteGo::LateUpdate(dt);
}

void Player::FixedUpdate(float dt)
{
    SpriteGo::FixedUpdate(dt);
}

void Player::Draw(sf::RenderWindow& window)
{
    //SpriteGo::Draw(window);
    window.draw(sprite);
}

void Player::HandleInput()
{
    if (isGrounded && !isJumpCharging) {
        jumpDirection = 0.f;
        if (InputMgr::GetKey(sf::Keyboard::Left)) {
            jumpDirection -= 1.f;
            sprite.setScale(-1.f, 1.f); // 왼쪽으로 이동 시 스프라이트 반전
        }
        if (InputMgr::GetKey(sf::Keyboard::Right)) {
            jumpDirection += 1.f;
            sprite.setScale(1.f, 1.f); // 오른쪽으로 이동 시 스프라이트 정방향
        }
        
        // 움직이는 중에는 차징을 시작할 수 없도록 함
        if ((jumpDirection == 0.f) && InputMgr::GetKeyDown(sf::Keyboard::Space)) {
            StartJumpCharging();
        }
    }

    // 차징 중 방향키 입력을 기반으로 점프 방향 설정
    if (isJumpCharging && InputMgr::GetKeyUp(sf::Keyboard::Space)) {
        if (InputMgr::GetKey(sf::Keyboard::Left)) {
            jumpDirection = -1;
        } else if (InputMgr::GetKey(sf::Keyboard::Right)) {
            jumpDirection = 1;
        }
        PerformJump();
    }
}

void Player::UpdateMovement(float dt)
{
    if (isJumping || isJumpCharging) {
        velocity.y += gravity * dt;
        position += velocity * dt;

        // 점프가 끝났을 때, 이동 및 점프 관련 변수 초기화
        if (position.y > groundYPosition) {
            position.y = groundYPosition;
            velocity.y = 0;
            isJumping = false;
            isGrounded = true;
            jumpDirection = 0; // 추가된 부분: 점프 후 방향 초기화
        }
    } else {
        // 차징 중이 아니면 이동 가능
        if (!isJumpCharging) {
            velocity.x = jumpDirection * moveSpeed;
            position += velocity * dt;
        }
    }
}

void Player::UpdateAnimation()
{
    // 점프 차징 중일 때의 애니메이션
    if (isJumpCharging) {
        if (animator.GetCurrentClipId() != "animations/player_Jump.csv") {
            animator.Play("animations/player_Jump.csv");
        }
    }
    // 공중에 있는 경우 (점프 중)
    else if (isJumping) {
        // 점프 상승 중
        if (velocity.y < 0 && animator.GetCurrentClipId() != "animations/player_JumpUp.csv") {
            animator.Play("animations/player_JumpUp.csv");
        }
        // 점프 하강 중
        else if (velocity.y > 0 && animator.GetCurrentClipId() != "animations/player_JumpDown.csv") {
            animator.Play("animations/player_JumpDown.csv");
        }
    }
    // 땅에 닿아 있고, 이동 중이지 않을 때
    else if (isGrounded && std::abs(velocity.x) < 0.1f) {
        if (animator.GetCurrentClipId() != "animations/player_Idle.csv") {
            animator.Play("animations/player_Idle.csv");
        }
    }
    // 땅에 닿아 있고, 이동 중일 때
    else if (isGrounded && std::abs(velocity.x) >= 0.1f) {
        if (animator.GetCurrentClipId() != "animations/player_Move.csv") {
            animator.Play("animations/player_Move.csv");
        }
    }
}

void Player::StartJumpCharging()
{
    isJumpCharging = true;
    jumpStartTime = timer.getElapsedTime().asSeconds();
}

void Player::PerformJump()
{
    isJumpCharging = false;
    isJumping = true;
    float chargeTime = std::min(timer.getElapsedTime().asSeconds() - jumpStartTime, maxjumpTime);
    
    // 점프 높이 설정
    velocity.y = -sqrt(8 * gravity * (100.f * chargeTime)); // 점프 높이 조절

    // 차징 시간에 따른 X축 속도 조절
    float jumpDurationFactor = std::min(chargeTime / maxjumpTime, 1.0f); // 0과 1 사이의 값을 가짐
    if(jumpDirection != 0) {
        velocity.x = jumpDirection * (moveSpeed * jumpDurationFactor * 0.5); // 차징 시간이 짧으면 X축 이동 거리 감소
    } else {
        velocity.x = 0; // 제자리 점프 시 좌우 이동 없음
    }
}

void Player::CheckCollision()
{
    
}
