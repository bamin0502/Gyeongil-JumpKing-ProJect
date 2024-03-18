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
    playerPhase=PlayerPhase::LANDING;
    lastPosY=position.y;
}

void Player::Reset() {
    gameScene = dynamic_cast<GameScene*>(SCENE_MGR.GetCurrentScene());
    collisionMap = gameScene->map1Sprite.getTexture()->copyToImage();
    animator.Play("animations/player_FallDown.csv");
    SetOrigin(Origins::BC);
    playerPhase = PlayerPhase::LANDING;
}

void Player::Update(float dt) {
    SpriteGo::Update(dt);
    animator.Update(dt);
    CheckCollision();
    UpdateMovement(dt);
    HandleInput(dt);
    if (playerPhase == PlayerPhase::GROUNDED) {
        isGrounded = true;
    } else {
        isGrounded = false;
    }
    if (playerPhase == PlayerPhase::LANDING) {
        if (InputMgr::GetKey(sf::Keyboard::Left) || InputMgr::GetKey(sf::Keyboard::Right) || timer.getElapsedTime().asSeconds() > 10.0f) {
            playerPhase = PlayerPhase::GROUNDED;
        }
    }
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

    if (showCollisionPoints) {
        for (const sf::Vector2f& point : collisionPoints) {
            window.draw(gameScene->map1Sprite);
            sf::CircleShape circle(2.f);
            circle.setFillColor(sf::Color::Black);
            circle.setOrigin(1.f, 1.f);
            circle.setPosition(point);
            window.draw(circle);
        }
    }
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
    if (InputMgr::GetKeyDown(sf::Keyboard::F1)) {
        showCollisionPoints = !showCollisionPoints; // F1 키가 눌리면 토글
    }
    // 자동 점프 체크
    float chargeDuration = timer.getElapsedTime().asSeconds() - jumpStartTime;
    if (isJumpCharging && chargeDuration > 0.6f) {
        PerformJump(true); // 자동 점프 실행
        isJumpCharging = false;
        playerPhase = PlayerPhase::JUMPING;
        return; 
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
            jumpDirection = 0.f; // 제자리
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
    
    if(!isGrounded)
    {
        velocity.y+=gravity*dt;
    }
    // 추락 상태 로직 개선
    if (!(static_cast<int>(collision) & static_cast<int>(CollisionType::BOTTOM))) {
        // 바닥 충돌이 없고, 점프 상태도 아니라면 추락 상태로 전환
        if (!isJumping) {
            playerPhase = PlayerPhase::FALLING;
        }
    } 
    // 점프 중이거나 공중에 있는 동안에는 좌우 이동 제한
    if (playerPhase != PlayerPhase::JUMPING && playerPhase != PlayerPhase::FALLING &&
        playerPhase != PlayerPhase::BOUNCE && playerPhase != PlayerPhase::CHARGING) {
        if (InputMgr::GetKey(sf::Keyboard::Left)) {
            currentPosition.x -= moveSpeed * dt;
            sprite.setScale(-1.f, 1.f); // 왼쪽을 바라보게 함
        }
        if (InputMgr::GetKey(sf::Keyboard::Right)) {
            currentPosition.x += moveSpeed * dt;
            sprite.setScale(1.f, 1.f); // 오른쪽을 바라보게 함
        }
    }
    if (playerPhase == PlayerPhase::GROUNDED) {
        isGrounded = true;
        if((static_cast<int>(collision)&static_cast<int>(CollisionType::LEFT))){
            CorrectLeftPosition(currentPosition,collision);
        }
        if((static_cast<int>(collision)&static_cast<int>(CollisionType::RIGHT))){
            CorrectRightPosition(currentPosition,collision);
        }
        
    }
    // UpdateMovement 내에서 바닥과의 충돌 처리 부분
    if(playerPhase == PlayerPhase::JUMPING)
    {
        velocity.x =jumpDirection*moveSpeed;
        currentPosition.x += velocity.x * dt;

        if(static_cast<int>(collision)&static_cast<int>(CollisionType::TOP))
        {
            CorrectTopPosition(currentPosition,collision);
            playerPhase=PlayerPhase::BOUNCE;
        }
        if(static_cast<int>(collision)&static_cast<int>(CollisionType::LEFT))
        {
            velocity.x=0;
            HandleWallBounce();
        }
        if(static_cast<int>(collision)&static_cast<int>(CollisionType::RIGHT))
        {
            velocity.x=0;
            HandleWallBounce();
        }
    }
    
    if(playerPhase==PlayerPhase::FALLING)
    {
        velocity.x =jumpDirection*moveSpeed;
        currentPosition.x += velocity.x * dt;
        isJumping = false;
        
        if(static_cast<int>(collision)&static_cast<int>(CollisionType::LEFT))
        {
            CorrectLeftPosition(currentPosition,collision);
            playerPhase=PlayerPhase::GROUNDED;
        }
        if(static_cast<int>(collision)&static_cast<int>(CollisionType::RIGHT))
        {
            CorrectRightPosition(currentPosition,collision);
            playerPhase=PlayerPhase::GROUNDED;
        }
        if(static_cast<int>(collision) & static_cast<int>(CollisionType::BOTTOM)) {
            CorrectBottomPosition(currentPosition, collision);
            playerPhase = PlayerPhase::GROUNDED;
        }
        
    }

    if(playerPhase==PlayerPhase::BOUNCE)
    {
        velocity.x =jumpDirection*moveSpeed;
        currentPosition.x += velocity.x * dt;
        
        if (static_cast<int>(collision) & static_cast<int>(CollisionType::LEFT)) {
            CorrectLeftPosition(currentPosition, collision);
        }
        if (static_cast<int>(collision) & static_cast<int>(CollisionType::RIGHT)) {
            CorrectRightPosition(currentPosition, collision);
        }
        if(static_cast<int>(collision)&static_cast<int>(CollisionType::TOP))
        {
            CorrectTopPosition(currentPosition,collision);
            playerPhase=PlayerPhase::FALLING;
            velocity.y=gravity*dt;
        }
        // 현재 위치를 업데이트합니다.
        //currentPosition.x += velocity.x * dt;
        currentPosition.y += velocity.y * dt;
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
        isJumping = true;
    }
    
    currentPosition.y += velocity.y * dt;
    SetPosition(currentPosition);
}

void Player::UpdateAnimation() {
    switch (playerPhase) {
    case PlayerPhase::CHARGING:
        if (animator.GetCurrentClipId() != "animations/player_Jump.csv") {
            animator.Play("animations/player_Jump.csv");
        }
        isGrounded = true;
        break;
    case PlayerPhase::JUMPING:
        if (animator.GetCurrentClipId() != "animations/player_JumpUp.csv") {
            animator.Play("animations/player_JumpUp.csv");
        }
        isGrounded = false;
        break;
    case PlayerPhase::FALLING:
        if (animator.GetCurrentClipId() != "animations/player_JumpDown.csv") {
            animator.Play("animations/player_JumpDown.csv");
        }
        isGrounded = false;
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
        isGrounded=true;
        break;
    case PlayerPhase::LANDING:
        if(animator.GetCurrentClipId()!="animations/player_FallDown.csv")
        {
            animator.Play("animations/player_FallDown.csv");
        }
        isGrounded=true;
        break;
    case PlayerPhase::BOUNCE:
        if(animator.GetCurrentClipId()!="animations/player_Bounce.csv")
        {
            animator.Play("animations/player_Bounce.csv");
        }
        isGrounded=false;
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

    // 점프 시작 위치 기록
    jumpStartPos = sprite.getPosition();
    
    if (playerPhase == PlayerPhase::JUMPING || playerPhase == PlayerPhase::FALLING) {
        if (const float distanceFallen = sprite.getPosition().y - jumpStartPos.y; distanceFallen >= 50.0f) {
            playerPhase = PlayerPhase::LANDING;
        }
    }
    isJumping = true;
    isGrounded = false;
    playerPhase = PlayerPhase::JUMPING;
    
    // 점프 초기 Y축 속도 설정: 높이에 기반한 계산
    velocity.y = -sqrt(0.95 * gravity * jumpHeight);
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
    // 충돌 검사 전 충돌 포인트 목록 초기화
    collisionPoints.clear();
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
    collisionPoints.push_back(points[0]);
    collisionPoints.push_back(points[1]);
    collisionPoints.push_back(points[2]);
    collisionPoints.push_back(points[3]);
    collisionPoints.push_back(bottomLeft);
    collisionPoints.push_back(bottomRight);
    

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
    velocity.y = 0;
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

    velocity.x=0;
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
    velocity.x=0;
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
    velocity.y = 0;
}

void Player::HandleWallBounce()
{
    if (isCollidingRight) {
        // 오른쪽 벽에 부딪히면 왼쪽으로 튕겨나가게 함
        velocity.x = -abs(velocity.x) * 1.5f; // 반대 방향으로 더 큰 속도로 반발력
    }
    if (isCollidingLeft) {
        // 왼쪽 벽에 부딪히면 오른쪽으로 튕겨나가게 함
        velocity.x = abs(velocity.x) * 1.5f; // 반대 방향으로 더 큰 속도로 반발력
    }
    // 사선으로 튕겨 올라가도록 수직 속도 조정
    velocity.y = -abs(velocity.y) * 0.25f; // 양수 값으로 설정하여 위로 튕겨나가게 함

    // BOUNCE 상태로 설정
    playerPhase = PlayerPhase::BOUNCE;
}





