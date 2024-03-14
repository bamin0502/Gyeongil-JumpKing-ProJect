#include "pch.h"
#include "Player.h"
#include "GameScene.h"

Player::Player(const std::string& name)
    : SpriteGo(name), collisionType(), playerPhase(PlayerPhase::Grounded), jumpHeight(0), jumpDirection(0.f),
      jumpGuage(0),
      currentJumpStage(0),
      currentHeight(0)
{
}

void Player::Init() {
    animator.SetTarget(&sprite);
    SetPosition({0.f, 205.f});
    playerPhase=PlayerPhase::Grounded;

    testRectangle.setSize(sf::Vector2f(50, 50)); // 테스트 사각형 크기 설정
    testRectangle.setFillColor(sf::Color::Green); // 색상 설정
    testRectangle.setPosition(0, 200); // 플레이어와 똑같은 초기 위치 설정
}

void Player::Release() {
    
}

void Player::Reset() {
    gameScene = dynamic_cast<GameScene*>(SCENE_MGR.GetCurrentScene());
    collisionMap = gameScene->map1Sprite.getTexture()->copyToImage();
    animator.Play("animations/player_FallDown.csv");
    SetOrigin(Origins::BC);

    playerPhase = PlayerPhase::Grounded;
}

void Player::Update(float dt) {
    SpriteGo::Update(dt);
    animator.Update(dt);
   
    CheckCollision();
    UpdateMovement(dt);
    HandleInput(dt);
    
    if(isJumping)
    {
        velocity.y+=gravity*dt;
        position.y+=velocity.y*dt;
    }
    
    if(testMode) {
        
    }
    if(InputMgr::GetKeyDown(sf::Keyboard::F1)){
        debugMode=!debugMode;
    }
    if(InputMgr::GetKeyDown(sf::Keyboard::F2)){
        testMode=!testMode;
    }
   
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
    if(!testMode)
    {
        window.draw(sprite);
    }
    if(debugMode)
    {
        //플레이어 바운딩 박스 그리기
        sf::RectangleShape boundingBox;
        boundingBox.setPosition(sprite.getPosition());
        boundingBox.setOrigin(sprite.getOrigin());
        sf::FloatRect bounds = sprite.getGlobalBounds();
        boundingBox.setSize(sf::Vector2f(bounds.width, bounds.height));
        boundingBox.setFillColor(sf::Color::Transparent);
        boundingBox.setOutlineColor(sf::Color::Black); // 빨간색으로 테두리를 그립니다.
        boundingBox.setOutlineThickness(1.f); // 테두리 두께 설정
        window.draw(boundingBox);
        window.draw(gameScene->map1Sprite);
    }
    if(testMode)
    {
        window.draw(gameScene->map1Sprite);
        window.draw(testRectangle);
    }

   
}

void Player::SetActive(const bool active)
{
    SpriteGo::SetActive(active);
}

void Player::HandleInput(float dt) {
    if(InputMgr::GetKeyDown(sf::Keyboard::Space))
    {
        if(isGrounded)
        {
            StartJumpCharging();
            playerPhase=PlayerPhase::Charging;
        }
    }
    if(InputMgr::GetKeyUp(sf::Keyboard::Space))
    {
        if(isJumpCharging)
        {
            PerformJump();
            playerPhase=PlayerPhase::Rising;
            isJumpCharging=false;
        }
    }
    
}

void Player::UpdateMovement(float dt)
{
    // 충돌 상태 업데이트
    CollisionType collision = CheckCollision();

    sf::Vector2f currentPosition = GetPosition();
    float moveDistance = moveSpeed * dt;
    sf::Vector2f moveDirection(0.f, 0.f);

    //isGrounded= CheckIfGrounded();
    
    if(isFalling)
    {
        if (!isGrounded) {
            velocity.y += gravity * dt; // 중력 값을 속도에 더함
        }else{
            velocity.y = 0;
        }
    }
    
    // 왼쪽 또는 오른쪽으로 이동하려 할 때 충돌 확인
    if (InputMgr::GetKey(sf::Keyboard::Left) && !(static_cast<int>(collision) & static_cast<int>(CollisionType::LEFT))) {
        moveDirection.x -= moveDistance;
        
    }
    if (InputMgr::GetKey(sf::Keyboard::Right) && !(static_cast<int>(collision) & static_cast<int>(CollisionType::RIGHT))) {
        moveDirection.x += moveDistance;
    }
    sprite.setScale(moveDirection.x < 0 ? -1.f : 1.f, 1.f);
    // 점프 중에 상단 충돌 확인
    if (isJumping && (static_cast<int>(collision) & static_cast<int>(CollisionType::TOP))){
        velocity.y = 0;
        isGrounded = false;
        isJumping = false;
        isFalling = true;
       
    }
    if(isFalling)
    {
        playerPhase=PlayerPhase::Falling;
    }
    // 하강 중에 하단 충돌 확인
    if (isFalling && (static_cast<int>(collision) & static_cast<int>(CollisionType::BOTTOM)))
    {
        velocity.y = 0;
        isGrounded = true;
        isFalling = false;
        isJumping = false;
        playerPhase=PlayerPhase::Grounded;
    }
    if(!isFalling && (static_cast<int>(collision) & static_cast<int>(CollisionType::BOTTOM)))
    {
        velocity.y = 0;
        isGrounded=true;
        isFalling=false;
        isJumping=false;
        playerPhase=PlayerPhase::Grounded;
    }
    moveDirection.y += velocity.y * dt;
    // 이동 및 충돌 처리 후의 새 위치 설정
    sf::Vector2f newPosition = currentPosition + moveDirection;
    SetPosition(newPosition);
}

void Player::UpdateAnimation() {
    switch (playerPhase) {
    case PlayerPhase::Charging:
        if (animator.GetCurrentClipId() != "animations/player_Jump.csv") {
            animator.Play("animations/player_Jump.csv");
        }
        break;
    case PlayerPhase::Rising:
        if (animator.GetCurrentClipId() != "animations/player_JumpUp.csv") {
            animator.Play("animations/player_JumpUp.csv");
        }
        break;
    case PlayerPhase::Falling:
        if (animator.GetCurrentClipId() != "animations/player_JumpDown.csv") {
            animator.Play("animations/player_JumpDown.csv");
        }
        break;
    case PlayerPhase::Grounded:
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
    playerPhase = PlayerPhase::Charging;
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
    isJumping = true;

    velocity.y=-sqrt(gravity*jumpHeight);
    velocity.x=jumpDirection*moveSpeed*0.5;

    sprite.setScale(jumpDirection<0?-1.f:1.f,1.f);
    animator.Play("animations/player_JumpUp.csv");
}



Player::CollisionType Player::CheckCollision()
{
    isCollidingTop=false;
    isCollidingBottom=false;
    isCollidingLeft=false;
    isCollidingRight=false;
    
    sf::FloatRect playerBounds = sprite.getGlobalBounds();
    
    sf::Vector2f points[4]={
        sf::Vector2f(playerBounds.left + playerBounds.width / 2, playerBounds.top),
        sf::Vector2f(playerBounds.left+playerBounds.width/2,playerBounds.top+playerBounds.height),
        sf::Vector2f(playerBounds.left,playerBounds.top+playerBounds.height/2),
        sf::Vector2f(playerBounds.left+playerBounds.width,playerBounds.top+playerBounds.height/2)
    };

    for (int i = 0; i < 4; ++i) {
        sf::Vector2f scenePos = gameScene->map1Sprite.getTransform().getInverse().transformPoint(points[i]);

        if (scenePos.x >= 0 && scenePos.y >= 0 && scenePos.x < collisionMap.getSize().x && scenePos.y < collisionMap.getSize().y) {
            sf::Color color = collisionMap.getPixel(static_cast<unsigned int>(scenePos.x), static_cast<unsigned int>(scenePos.y));

            // 여기에서는 예시로 Red와 Blue 색상을 충돌로 간주
            if (color == sf::Color::Red || color == sf::Color::Blue) {
                switch (i) {
                case 0: isCollidingTop = true; break;
                case 1: isCollidingBottom = true; break;
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


bool Player::CheckGroundCollision()
{
    return false;
}

bool Player::CheckWallCollision()
{
    return false;
}

// int Player::CheckCollisionWithMap()
// {
//     sf::FloatRect playerBounds = sprite.getGlobalBounds();
//     // 플레이어 바운딩 박스의 하단 중앙점에서 시작
//     int startX = static_cast<int>(playerBounds.left + playerBounds.width / 2);
//     int startY = static_cast<int>(playerBounds.top + playerBounds.height);
//
//     // collisionMap의 높이까지 y 좌표를 증가시키며 검사
//     for (int y = startY; y < collisionMap.getSize().y; ++y) {
//         sf::Color pixelColor = collisionMap.getPixel(startX, y);
//
//         // 바닥을 나타내는 픽셀을 찾음 (예: Red 또는 Blue)
//         if (pixelColor == sf::Color::Red || pixelColor == sf::Color::Blue) {
//             // 찾은 픽셀의 y 좌표 반환
//             return y;
//         }
//     }
//
//     // 바닥을 찾지 못한 경우
//     return -1;
// }


// sf::Vector2f Player::GetGroundCheckPosition()
// {
//     sf::FloatRect playerBounds = sprite.getGlobalBounds();
//     sf::Vector2f groundCheckPosition(playerBounds.left + playerBounds.width / 2, playerBounds.top + playerBounds.height);
//     return groundCheckPosition;
// }


// bool Player::CheckIfGrounded()
// {
//     sf::Vector2f groundCheckPosition=GetGroundCheckPosition();
//     
//     bool onGround = true;
//     return onGround;
// }
//
// sf::Vector2f Player::GetGroundCheckPosition()
// {
//     sf::FloatRect playerBounds = sprite.getGlobalBounds();
//     sf::Vector2f groundCheckPosition(playerBounds.left + playerBounds.width / 2, playerBounds.top + playerBounds.height);
//
//     return groundCheckPosition;
// }





