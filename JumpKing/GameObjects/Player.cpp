#include "pch.h"
#include "Player.h"
#include "GameScene.h"

Player::Player(const std::string& name)
    : SpriteGo(name), collisionType(), playerPhase(PlayerPhase::GROUNDED), jumpHeight(0), jumpDirection(0.f),
      jumpGuage(0),
      currentJumpStage(0),
      currentHeight(0), lastPosY(0), initialJumpVelocity(0)
{
}

void Player::Init() {
    animator.SetTarget(&sprite);
    SetPosition({0.f, 210.f});
    playerPhase=PlayerPhase::GROUNDED;
    lastPosY=position.y;
}

void Player::Reset() {
    gameScene = dynamic_cast<GameScene*>(SCENE_MGR.GetCurrentScene());
    collisionMap = gameScene->map1Sprite.getTexture()->copyToImage();
    animator.Play("animations/player_FallDown.csv");
    SetOrigin(Origins::BC);

    playerPhase = PlayerPhase::GROUNDED;
}

void Player::Update(float dt) {
    SpriteGo::Update(dt);
    animator.Update(dt);
   
    CheckCollision();
    UpdateMovement(dt);
    HandleInput(dt);
    
   
    // 위치 업데이트 후 추락 체크
    if (position.y > lastPosY) {
        //부딪히지 않았디면
        if (!isCollidingLeft && !isCollidingRight && !isCollidingTop && !isCollidingBottom) {
            playerPhase = PlayerPhase::FALLING;
        }
    }
    lastPosY = position.y;
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

void Player::SetActive(const bool active)
{
    SpriteGo::SetActive(active);
}

void Player::HandleInput(float dt) {

    if(playerPhase==PlayerPhase::JUMPING || playerPhase==PlayerPhase::FALLING || playerPhase==PlayerPhase::BOUNCE)
    {
        return;
    }
    
    // 자동 점프 체크
    float chargeDuration = timer.getElapsedTime().asSeconds() - jumpStartTime;
    if (isJumpCharging && chargeDuration > 0.6f) {
        PerformJump(true); // 자동 점프 실행
        isJumpCharging = false;
        playerPhase = PlayerPhase::JUMPING;
        return; // 점프가 실행되었으므로 이후 입력 처리를 건너뜁니다
    }
    if (playerPhase == PlayerPhase::GROUNDED && InputMgr::GetKeyDown(sf::Keyboard::Space)) {
        StartJumpCharging(); // 점프 차징 시작
    } else if (playerPhase == PlayerPhase::CHARGING) {
        // 차징 중 방향 결정
        if (InputMgr::GetKey(sf::Keyboard::Left)) {
            jumpDirection = -1.f; // 왼쪽 방향
        } else if (InputMgr::GetKey(sf::Keyboard::Right)) {
            jumpDirection = 1.f; // 오른쪽 방향
        } else {
            jumpDirection = 0; // 제자리
        }
    }
    
    if (isJumpCharging && InputMgr::GetKeyUp(sf::Keyboard::Space)) {
        PerformJump(false); // 스페이스바를 뗐을 때 점프 실행
        isJumpCharging = false;
        playerPhase = PlayerPhase::JUMPING;
        isJumping = true;
    }
}

void Player::UpdateMovement(float dt)
{
    CollisionType collision = CheckCollision();
    sf::Vector2f currentPosition = GetPosition();
    const float moveDistance = this->moveSpeed * dt;

    // Grounded 상태에서만 방향키 입력에 따라 이동
    if (playerPhase == PlayerPhase::GROUNDED) {
        if (InputMgr::GetKey(sf::Keyboard::Left)) {
            currentPosition.x -= moveSpeed * dt;
            sprite.setScale(-1.f, 1.f); // 왼쪽을 바라보게 함
        }
        if (InputMgr::GetKey(sf::Keyboard::Right)) {
            currentPosition.x += moveSpeed * dt;
            sprite.setScale(1.f, 1.f); // 오른쪽을 바라보게 함
        }
        if((static_cast<int>(collision)&static_cast<int>(CollisionType::LEFT))){
            CorrectLeftPosition(currentPosition,collision);    
        }
        if((static_cast<int>(collision)&static_cast<int>(CollisionType::RIGHT))){
            CorrectRightPosition(currentPosition,collision);
        }
    }
    
    
    if(playerPhase ==PlayerPhase::JUMPING)
    {
        velocity.x =jumpDirection*moveSpeed;
        currentPosition.x += velocity.x * dt;

        if(static_cast<int>(collision)&static_cast<int>(CollisionType::TOP))
        {
            CorrectTopPosition(currentPosition,collision);
            playerPhase=PlayerPhase::FALLING;
        }
        else if(static_cast<int>(collision)&static_cast<int>(CollisionType::LEFT))
        {
            CorrectLeftPosition(currentPosition,collision);
            playerPhase=PlayerPhase::FALLING;
        }
        else if(static_cast<int>(collision)&static_cast<int>(CollisionType::RIGHT))
        {
            CorrectRightPosition(currentPosition,collision);
            playerPhase=PlayerPhase::FALLING;
        }

    }
    
    if(playerPhase==PlayerPhase::FALLING)
    {
        velocity.x =jumpDirection*moveSpeed;
        currentPosition.x += velocity.x * dt;
        velocity.y+=gravity*dt;
        if(static_cast<int>(collision)&static_cast<int>(CollisionType::BOTTOM))
        {
            CorrectBottomPosition(currentPosition,collision);
            playerPhase=PlayerPhase::GROUNDED;
        }
        
       
    }

    if(playerPhase==PlayerPhase::BOUNCE)
    {
        if(static_cast<int>(collision)&static_cast<int>(CollisionType::BOTTOM))
        {
            CorrectBottomPosition(currentPosition,collision);
            playerPhase=PlayerPhase::GROUNDED;
        }
        else if(static_cast<int>(collision)&static_cast<int>(CollisionType::LEFT))
        {
            CorrectLeftPosition(currentPosition,collision);
            
        }
        else if(static_cast<int>(collision)&static_cast<int>(CollisionType::RIGHT))
        {
            CorrectRightPosition(currentPosition,collision);
            
        }
    }

       
    if (playerPhase == PlayerPhase::CHARGING) {
        if(InputMgr::GetKey(sf::Keyboard::Left))
        {
            jumpDirection=-1.25f;
        }
        else if(InputMgr::GetKey(sf::Keyboard::Right))
        {
            jumpDirection=1.25f;
        }
        else
        {
            jumpDirection=0;
        }
    }
    
    if(isJumping)
    {
        if(isCollidingLeft && playerPhase==PlayerPhase::JUMPING)
        {
            HandleWallBounce();
        }
        if(isCollidingRight && playerPhase==PlayerPhase::JUMPING)
        {
            HandleWallBounce();
        }
        
        if(isCollidingTop)
        {
            velocity.y = gravity * dt;
            playerPhase=PlayerPhase::BOUNCE;
            isJumping=false;
        }
        velocity.y+=gravity*dt;
    }
    
    
  
    
    //currentPosition.x += velocity.x * dt;
    currentPosition.y += velocity.y * dt;
    SetPosition(currentPosition);
    
    UpdateAnimation();
    
}

void Player::UpdateAnimation() {
    switch (playerPhase) {
    case PlayerPhase::CHARGING:
        if (animator.GetCurrentClipId() != "animations/player_Jump.csv") {
            animator.Play("animations/player_Jump.csv");
        }
        break;
    case PlayerPhase::JUMPING:
        if (animator.GetCurrentClipId() != "animations/player_JumpUp.csv") {
            animator.Play("animations/player_JumpUp.csv");
        }
        break;
    case PlayerPhase::FALLING:
        if (animator.GetCurrentClipId() != "animations/player_JumpDown.csv") {
            animator.Play("animations/player_JumpDown.csv");
        }
        break;
    case PlayerPhase::GROUNDED:
        if(InputMgr::GetKey(sf::Keyboard::Left)||InputMgr::GetKey(sf::Keyboard::Right))
        {
            if(animator.GetCurrentClipId()!="animations/player_Move.csv")
            {
                animator.Play("animations/player_Move.csv");
            }
        }
        else
        {
            if(animator.GetCurrentClipId()!="animations/player_Idle.csv")
            {
                animator.Play("animations/player_Idle.csv");
            }
        }
        break;
    case PlayerPhase::LANDING:
        if(animator.GetCurrentClipId()!="animations/player_FallDown.csv")
        {
            animator.Play("animations/player_FallDown.csv");
        }
        break;
    case PlayerPhase::BOUNCE:
        if(animator.GetCurrentClipId()!="animations/player_Bounce.csv")
        {
            animator.Play("animations/player_Bounce.csv");
        }
        break;
    default: ;
    }
}

void Player::StartJumpCharging() {
    isJumpCharging = true;
    jumpStartTime = timer.getElapsedTime().asSeconds();
    playerPhase = PlayerPhase::CHARGING;
}

float calculateJumpHeight(float chargeDuration, float maxJumpHeight)
{
    constexpr int levels = 35;
    constexpr float maxChargeTime = 0.6f;
    constexpr float levelDuration = maxChargeTime / levels;
    const int level = std::min(static_cast<int>(chargeDuration / levelDuration), levels);
    const float jumpHeight = maxJumpHeight * (level / static_cast<float>(levels));

    return jumpHeight;
}
void Player::PerformJump(bool isAutoJump) {
    const float chargeDuration = timer.getElapsedTime().asSeconds() - jumpStartTime;
    float jumpHeight;

    if (isAutoJump) {
        jumpHeight = maxJumpHeight; // 자동 점프 시 최대 높이
    } else {
        jumpHeight = calculateJumpHeight(chargeDuration, maxJumpHeight);
    }

    isJumping = true;
    playerPhase = PlayerPhase::JUMPING;
    
    // 점프 초기 Y축 속도 설정: 높이에 기반한 계산
    velocity.y = -sqrt(1 * gravity * jumpHeight);
    // 점프 방향에 따른 X축 속도 설정
    velocity.x = 0.5*jumpDirection * moveSpeed;

    // 방향에 따라 스프라이트 반전
    if(jumpDirection != 0) { // 방향이 설정되어 있으면
        sprite.setScale(jumpDirection < 0 ? -1.f : 1.f, 1.f);
    }
}


Player::CollisionType Player::CheckCollision()
{
    isCollidingTop=false;
    isCollidingBottom=false;
    isCollidingLeft=false;
    isCollidingRight=false;

    const sf::FloatRect playerBounds = sprite.getGlobalBounds();
    float quarterWidth=playerBounds.width/4.f;

    sf::Vector2f bottomLeft(playerBounds.left+quarterWidth,playerBounds.top+playerBounds.height);
    sf::Vector2f bottomRight(playerBounds.left + 3 * quarterWidth, playerBounds.top + playerBounds.height);

    const sf::Vector2f points[6]={
        sf::Vector2f(playerBounds.left + playerBounds.width / 2, playerBounds.top),
        sf::Vector2f(playerBounds.left+playerBounds.width/2,playerBounds.top+playerBounds.height),
        sf::Vector2f(playerBounds.left,playerBounds.top+playerBounds.height/2),
        sf::Vector2f(playerBounds.left+playerBounds.width,playerBounds.top+playerBounds.height/2),
        bottomLeft,
        bottomRight
    };

    for (int i = 0; i < 6; ++i) {
        const sf::Vector2f scenePos = gameScene->map1Sprite.getTransform().getInverse().transformPoint(points[i]);

        if (scenePos.x >= 0 && scenePos.y >= 0 && scenePos.x < collisionMap.getSize().x && scenePos.y < collisionMap.getSize().y) {
            sf::Color color = collisionMap.getPixel(static_cast<unsigned int>(scenePos.x), static_cast<unsigned int>(scenePos.y));
            
            if (color == sf::Color::Red || color == sf::Color::Blue) {
                switch (i) {
                case 0: isCollidingTop = true; break;
                case 1: case 4: case 5: isCollidingBottom = true; break;
                case 2: isCollidingLeft = true; break;
                case 3: isCollidingRight = true; break;
                default: ;
                }
            }
        }
    }
    CollisionType result = CollisionType::NONE;
    if (isCollidingTop) {
        result = static_cast<CollisionType>(static_cast<int>(result) | static_cast<int>(CollisionType::TOP));
    }
    if (isCollidingBottom) {
        result = static_cast<CollisionType>(static_cast<int>(result) | static_cast<int>(CollisionType::BOTTOM));
    }
    if (isCollidingLeft) {
        result = static_cast<CollisionType>(static_cast<int>(result) | static_cast<int>(CollisionType::LEFT));
    }
    if (isCollidingRight) {
        result = static_cast<CollisionType>(static_cast<int>(result) | static_cast<int>(CollisionType::RIGHT));
    }
    return result;
}

void Player::CorrectBottomPosition(sf::Vector2f& currentPosition, CollisionType collision)
{
    float correctedY = currentPosition.y;
   
    collision = CheckCollision();
    while (static_cast<int>(collision) & static_cast<int>(CollisionType::BOTTOM)) {
        correctedY -= 1.0f; 
        currentPosition.y = correctedY; 
        SetPosition(currentPosition);
        collision = CheckCollision();
        
    }
    correctedY += 1.0f;
    
    currentPosition.y = correctedY;
    SetPosition(currentPosition);
    playerPhase = PlayerPhase::GROUNDED;
    velocity.y = 0;
    isGrounded = true;
    isJumping = false; 
}

void Player::CorrectRightPosition(sf::Vector2f& currentPosition, CollisionType collision)
{
    float correctedX=currentPosition.x;

    collision=CheckCollision();
    while(static_cast<int>(collision)&static_cast<int>(CollisionType::RIGHT))
    {
        correctedX-=1.0f;
        currentPosition.x=correctedX;
        SetPosition(currentPosition);
        collision=CheckCollision();
    }
    correctedX+=1.0f;

    currentPosition.x=correctedX;
    SetPosition(currentPosition);
    playerPhase=PlayerPhase::GROUNDED;
    velocity.x=0;
    isGrounded=false;
    isJumping=false;
}

void Player::CorrectLeftPosition(sf::Vector2f& currentPosition, CollisionType collision)
{
    float correctedX=currentPosition.x;

    collision=CheckCollision();
    while(static_cast<int>(collision)&static_cast<int>(CollisionType::LEFT))
    {
        correctedX+=1.0f;
        currentPosition.x=correctedX;
        SetPosition(currentPosition);
        collision=CheckCollision();
    }
    correctedX-=1.0f;

    currentPosition.x=correctedX;
    SetPosition(currentPosition);
    playerPhase=PlayerPhase::GROUNDED;
    velocity.x=0;
    isGrounded=false;
    isJumping=false;
    
}

void Player::CorrectTopPosition(sf::Vector2f& currentPosition, CollisionType collision)
{
    float correctedY = currentPosition.y;

    collision = CheckCollision();
    while (static_cast<int>(collision) & static_cast<int>(CollisionType::TOP)) {
        correctedY += 1.0f;
        currentPosition.y = correctedY;
        SetPosition(currentPosition);
        collision = CheckCollision();
    }
    correctedY -= 1.0f;

    currentPosition.y = correctedY;
    SetPosition(currentPosition);
    playerPhase = PlayerPhase::GROUNDED;
    velocity.y = 0;
    isGrounded = false;
    isJumping = false;
    
}

void Player::HandleWallBounce()
{
    float bounceSpeed = 200.f; // 벽에 부딪힐 때의 반발력
    float bounceUpSpeed = 100.f; // 벽에 부딪힌 후 상승하는 속도
    if (isCollidingRight) {
        velocity.x = -bounceSpeed; // 오른쪽 벽에 부딪혔을 때 왼쪽으로 반발력
    } else if (isCollidingLeft) {
        velocity.x = bounceSpeed; // 왼쪽 벽에 부딪혔을 때 오른쪽으로 반발력
    }
    velocity.y = -bounceUpSpeed; // 살짝 상승한 후 다시 떨어지도록 Y축 속도 조정

    playerPhase = PlayerPhase::BOUNCE; // 플레이어 상태를 BOUNCE로 변경
}


