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
        const sf::FloatRect bounds = sprite.getGlobalBounds();
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
        if(playerPhase==PlayerPhase::Grounded)
        {
            StartJumpCharging();
        }
    }
    if (isJumpCharging) {
        // 자동 점프 실행
        if (timer.getElapsedTime().asSeconds() - jumpStartTime >= 1.5f) {
            PerformJump();
        }
    }
    if(InputMgr::GetKeyUp(sf::Keyboard::Space))
    {
        if(isJumpCharging)
        {
            PerformJump();
            isJumpCharging=false;
        }
    }
    if (playerPhase == PlayerPhase::Charging) {
        if (InputMgr::GetKey(sf::Keyboard::Left)) {
            jumpDirection = -1; // 왼쪽 방향 설정
        } else if (InputMgr::GetKey(sf::Keyboard::Right)) {
            jumpDirection = 1; // 오른쪽 방향 설정
        }
    }
}

void Player::UpdateMovement(float dt)
{
    CollisionType collision = CheckCollision();
    sf::Vector2f currentPosition = GetPosition();
    const float moveDistance = moveSpeed * dt;

    // X축 이동 처리
    if (InputMgr::GetKey(sf::Keyboard::Left) && !(static_cast<int>(collision) & static_cast<int>(CollisionType::LEFT))) {
        currentPosition.x -= moveDistance;

        sprite.setScale(-1.f,1.f);
    }
    if (InputMgr::GetKey(sf::Keyboard::Right) && !(static_cast<int>(collision) & static_cast<int>(CollisionType::RIGHT))) {
        currentPosition.x += moveDistance;

        sprite.setScale(1.f,1.f);
    }

    // 중력 적용
    if (!(static_cast<int>(collision) & static_cast<int>(CollisionType::BOTTOM))) {
        velocity.y += gravity * dt;
    } else {
        CorrectYposition(currentPosition, collision);
        playerPhase = PlayerPhase::Grounded;
    }

    currentPosition.y += velocity.y * dt;
    SetPosition(currentPosition);

    UpdateAnimation();
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
    case PlayerPhase::Landing:
        if(animator.GetCurrentClipId()!="animations/player_FallDown.csv")
        {
            animator.Play("animations/player_FallDown.csv");
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
    playerPhase=PlayerPhase::Rising;
    velocity.y=-sqrt(chargeDuration*jumpHeight*1*gravity);
    velocity.x=jumpDirection*moveSpeed*0.5;

    sprite.setScale(jumpDirection<0?-1.f:1.f,1.f);
    animator.Play("animations/player_JumpUp.csv");
    timer.restart();
}


Player::CollisionType Player::CheckCollision()
{
    isCollidingTop=false;
    isCollidingBottom=false;
    isCollidingLeft=false;
    isCollidingRight=false;

    const sf::FloatRect playerBounds = sprite.getGlobalBounds();

    const sf::Vector2f points[4]={
        sf::Vector2f(playerBounds.left + playerBounds.width / 2, playerBounds.top),
        sf::Vector2f(playerBounds.left+playerBounds.width/2,playerBounds.top+playerBounds.height),
        sf::Vector2f(playerBounds.left,playerBounds.top+playerBounds.height/2),
        sf::Vector2f(playerBounds.left+playerBounds.width,playerBounds.top+playerBounds.height/2)
    };

    for (int i = 0; i < 4; ++i) {
        const sf::Vector2f scenePos = gameScene->map1Sprite.getTransform().getInverse().transformPoint(points[i]);

        if (scenePos.x >= 0 && scenePos.y >= 0 && scenePos.x < collisionMap.getSize().x && scenePos.y < collisionMap.getSize().y) {
            sf::Color color = collisionMap.getPixel(static_cast<unsigned int>(scenePos.x), static_cast<unsigned int>(scenePos.y));
            
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

void Player::CorrectYposition(sf::Vector2f& currentPosition, CollisionType collision)
{
    float correctedY = currentPosition.y;
   
    collision = CheckCollision();
    while (static_cast<int>(collision) & static_cast<int>(CollisionType::BOTTOM)) {
        correctedY -= 1.0f; 
        currentPosition.y = correctedY; 
        SetPosition(currentPosition);
        collision = CheckCollision();
    }
    // 충돌이 없는 위치를 찾았으면, 한 픽셀 아래로 이동하여 바닥에 "딱" 맞춥니다.
    correctedY += 1.0f;
    currentPosition.y = correctedY;
    SetPosition(currentPosition);
    
    velocity.y = 0;
    isGrounded = true;
    isJumping = false; 
}


