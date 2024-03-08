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
}

void Player::Update(float dt)
{
    SpriteGo::Update(dt);

    animator.Update(dt);

    auto prevVel = velocity;
    
    if (isJumpCharging)
    {
        if (InputMgr::GetKeyUp(sf::Keyboard::Space))
        {
            isJumping = true;
            isJumpCharging = false;
            float chargeTime = timer.getElapsedTime().asSeconds() - jumpStartTime;
            float jumpPower = 35.f * (chargeTime / maxjumpTime);
            velocity.y = -sqrt(2 * gravity * jumpPower);
            float jumpDirection = 0.f;
            if (InputMgr::GetKey(sf::Keyboard::Left))
            {
                jumpDirection -= 1.f;
            }
            if (InputMgr::GetKey(sf::Keyboard::Right))
            {
                jumpDirection += 1.f;
            }
            velocity.x = jumpDirection * moveSpeed * (chargeTime / maxjumpTime);
            animator.Play("animations/player_JumpUp.csv");
        }
    }
    else if (isJumping)
    {
        velocity.y += gravity * dt;
        if (prevVel.y > 0 && velocity.y < 0)
        {
            animator.Play("animations/player_JumpDown.csv");
        }
        if(!animator.IsPlaying())
        {
            animator.Play("animations/player_Idle.csv");
        }
    }
    else if (isGrounded)
    {
        if (InputMgr::GetKeyDown(sf::Keyboard::Space))
        {
            isGrounded = false;
            isJumpCharging = true;
            jumpStartTime = timer.getElapsedTime().asSeconds();
            animator.Play("animations/player_Jump.csv");
        }
    }
    
    position += velocity * dt;
    if (position.y > 0)
    {
        velocity.y = 0;
        isGrounded = true;

        isJumping = false;
        isJumpCharging = false;
        animator.Play("animations/player_Idle.csv");
    }
    // 이동 애니메이션 설정
    if (!isJumpCharging && !isJumping) {
        float h = 0.f;
        if (InputMgr::GetKey(sf::Keyboard::Left))
        {
            h -= 1.f;
        }
        if (InputMgr::GetKey(sf::Keyboard::Right))
        {
            h += 1.f;
        }
        velocity.x = h * moveSpeed;
        if (h != 0.f)
        {
            SetFlipX(h < 0);
        }
        if (animator.GetCurrentClipId() == "animations/player_Idle.csv")
        {
            if (h != 0)
            {
                animator.Play("animations/player_Move.csv");
            }
        }
        else if (animator.GetCurrentClipId() == "animations/player_Move.csv")
        {
            if (h == 0)
            {
                animator.Play("animations/player_Idle.csv");
            }
        }
    }
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

void Player::CheckCollision()
{
    
}
