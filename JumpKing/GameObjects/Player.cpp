﻿#include "pch.h"
#include "Player.h"

Player::Player(const std::string& name)
    : SpriteGo(name)
{
}

Player::~Player()
{
}

void Player::Init()
{
    SpriteGo::Init();
    
    animator.SetTarget(&sprite);
    

    
}

void Player::Release()
{
    SpriteGo::Release();
}

void Player::Reset()
{
    SpriteGo::Reset();

    animator.Play("animations/player_Idle.csv");
    SetOrigin(Origins::BC);
    SetPosition({0.f,0.f});
    
}

void Player::Update(float dt)
{
    SpriteGo::Update(dt);

    animator.Update(dt);
    //이동 관련 코드 
    float h=InputMgr::GetAxis(Axis::Horizontal);
    velocity.x=moveSpeed*h;
    
    //점프 관련 코드 0.6초동안 35단계로 점프력을 결정 일단은 보류
    if(isGrounded && InputMgr::GetKeyDown(sf::Keyboard::Space))
    {
        isGrounded = false;
        //뒤에 애니메이션 추가 예정
        //animator.Play("animations/Jump.csv");
        velocity.y = -sqrtf(2.f * gravity * 100);
    }
    velocity.y+=gravity*dt;

    position+=velocity*dt;
    if(position.y>0.f)
    {
        isGrounded=true;

        position.y=0.f;
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
    SpriteGo::Draw(window);
    window.draw(sprite);
}

void Player::Jump()
{
    
}

void Player::Move(float dt)
{
    
}

void Player::CheckCollision()
{
    
}
